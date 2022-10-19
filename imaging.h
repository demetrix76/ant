#pragma once

#include <cstdint>
#include <type_traits>
#include <vector>
#include <limits>
#include <algorithm>
#include <iterator>
#include <cstdlib>

namespace imaging
{
    /** RGBA пиксель */
    struct PixelRGBA
    {
        constexpr PixelRGBA() noexcept
        {}

        constexpr PixelRGBA(std::uint8_t aR, std::uint8_t aG, std::uint8_t aB, std::uint8_t aA = CHANNEL1) noexcept:
            r(aR), g(aG), b(aB), a(aA)
        {}

        constexpr PixelRGBA(std::uint8_t aGray, std::uint8_t aAlpha = CHANNEL1) noexcept:
            r(aGray), g(aGray), b(aGray), a(aAlpha)
        {}

        bool operator == (PixelRGBA const & aOther) const noexcept
        {
            return r == aOther.r
                && g == aOther.g
                && b == aOther.b
                && a == aOther.a;
        }

        std::uint8_t    r{};
        std::uint8_t    g{};
        std::uint8_t    b{};
        std::uint8_t    a{};

        static constexpr std::uint8_t CHANNEL0 = 0u;
        static constexpr std::uint8_t CHANNEL1 = std::numeric_limits<std::uint8_t>::max();
    };

    static constexpr PixelRGBA PixelRED   { PixelRGBA::CHANNEL1, PixelRGBA::CHANNEL0, PixelRGBA::CHANNEL0 };
    static constexpr PixelRGBA PixelGREEN { PixelRGBA::CHANNEL0, PixelRGBA::CHANNEL1, PixelRGBA::CHANNEL0 };
    static constexpr PixelRGBA PixelBLUE  { PixelRGBA::CHANNEL0, PixelRGBA::CHANNEL0, PixelRGBA::CHANNEL1 };
    static constexpr PixelRGBA PixelBLACK { PixelRGBA::CHANNEL0, PixelRGBA::CHANNEL0, PixelRGBA::CHANNEL0 };
    static constexpr PixelRGBA PixelWHITE { PixelRGBA::CHANNEL1, PixelRGBA::CHANNEL1, PixelRGBA::CHANNEL1 };


    /* Проверки, что мы не допустили вольностей при определении pixel:
       - StandardLayoutType: стандарт позволяет нам преобразовывать указатель на pixel в указатель на первый non-static data member
       - is_nothrow_move_constructible_v: важно, если мы используем std::vector<pixel>
     */
    static_assert(std::is_standard_layout_v<PixelRGBA>, "Pixel must be a StandardLayout type");
    static_assert(std::is_nothrow_move_constructible_v<PixelRGBA>, "PixelRGBA must be nothrow move constructible");
    static_assert(std::is_nothrow_move_assignable_v<PixelRGBA>, "PixelRGBA must be nothrow move assignable");

    /** Растровое изображение; шаблон класса, параметризуемый типом пикселя     
    */
    template<typename pixel_t>
    class Image
    {
    public:
        /** Создаёт объект Image размером Width * Height и заполняет заданным цветом 
        */
        Image(unsigned aWidth, unsigned aHeight, pixel_t const & aFill = pixel_t{}):
            mPixels(aWidth * aHeight, aFill),
            mWidth(aWidth),
            mHeight(aHeight)
        {}

        /** Создаёт объект Image размером Width * Height; цвет пикселя определяется функцией (x, y) => Pixel
        */
        template<typename function_t, typename _ = std::invoke_result_t<function_t, int, int>>
        Image(unsigned aWidth, unsigned aHeight, function_t && aFunction) :
            mWidth(aWidth),
            mHeight(aHeight)
        {
            std::size_t size = (std::size_t)mWidth * (std::size_t)mHeight;
            mPixels.reserve(size);
            std::generate_n(std::back_inserter(mPixels), size, 
                [h = mHeight - 1, w = mWidth, ind = 0u, fun = std::forward<function_t>(aFunction)]() mutable {
                    auto [dy, x] = std::div(int(ind++), int(w));
                    return fun(x, h - dy);
                }
            );
        }

        pixel_t & pixel(unsigned aX, unsigned aY) //< Пиксель по заданным координатам
        {
            if(aX >= mWidth || aY >= mHeight)
                throw std::runtime_error("Pixel coordinate out of bounds");
            return mPixels[(mHeight -1 - aY) * mWidth + aX];
        }

        pixel_t const & pixel(unsigned aX, unsigned aY) const //< Пиксель по заданным координатам
        {
            return const_cast<Image*>(this)->pixel(aX, aY);
        }

        unsigned width() const noexcept { return mWidth; } //< Ширина изоборажения в пикселях
        unsigned height() const noexcept { return mHeight; } //< Высота изображения в пикселях

        std::vector<pixel_t const*> rowPointers() const //< Для PNGLIB
        {
            if(!(mHeight && mWidth))
                return {};

            std::vector<pixel_t const*> pointers;
            pointers.reserve(mHeight);
            std::generate_n(std::back_inserter(pointers), mHeight,
                [step = mWidth, ptr = &mPixels.front()]() mutable { 
                    auto rv = ptr; ptr+= step; return rv;
                }
            );
            return pointers;
        }

    private:
        std::vector<pixel_t>    mPixels;
        unsigned                mWidth;
        unsigned                mHeight;
    };

    using ImageRGBA = Image<PixelRGBA>;

}