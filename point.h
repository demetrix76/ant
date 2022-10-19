#pragma once

#include <type_traits>
#include <functional>

namespace Ant
{
    /// 2D вектор
    struct Point
    {
        int x;
        int y;

        bool operator == (Point const & aOther) const noexcept
        {
            return x == aOther.x && y == aOther.y;
        }

        Point up() const { return Point { x, y + 1}; }
        Point down() const { return Point { x, y - 1}; }
        Point left() const { return Point { x - 1, y}; }
        Point right() const { return Point { x + 1, y}; }
    };
}

namespace std
{
    /** Специализация std::hash для Ant::Point. Использован модифицированный алгоритм,
     *  предложенный Matthew Szudzik http://szudzik.com/ElegantPairing.pdf
     *  https://stackoverflow.com/questions/919612/mapping-two-integers-to-one-in-a-unique-and-deterministic-way
    */
    template<>
    struct hash<Ant::Point>
    {
        std::size_t operator () (Ant::Point const & aPoint) const noexcept
        {
            return hashSzudzik(aPoint.x, aPoint.y);
        }

    private:
        static long long hashSzudzik(int a, int b) noexcept
        {
            auto A = (unsigned long long)(a >= 0 ? 2 * (long long)a : -2 * (long long)a - 1);
            auto B = (unsigned long long)(b >= 0 ? 2 * (long long)b : -2 * (long long)b - 1);
            auto C = (long long)((A >= B ? A * A + A + B : A + B * B) / 2);
            return a < 0 && b < 0 || a >= 0 && b >= 0 ? C : -C - 1;
        }

    };

}

