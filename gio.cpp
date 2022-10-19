#include "gio.h"

#include "finally.h"
#include <png.h>
#include <csetjmp>

#if defined(__linux__) || defined (__APPLE__)
#include <unistd.h>
#elif defined(_WIN32)

#endif

namespace gio
{

    bool pngWrite(imaging::Image<imaging::PixelRGBA> const & aImage, FILE * aFile)
    {
        if(!aFile)
            return false;

        auto row_pointers = aImage.rowPointers();
        if(row_pointers.empty())
            return false;
        
        png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
        png_infop info = nullptr;
        if(!png)
            return false;

        info = png_create_info_struct(png);
        if(!info)
            return false;

        handy::finally fin_png{[=]() mutable { png_destroy_write_struct(&png, &info); }};

        if (setjmp(png_jmpbuf(png)))
        {
            // Таким образом LibPNG сигнализирует об ошибке.
            // Достаточно тонкий момент, но если я правильно понимаю стандарт,
            // longjmp() не создаст проблем с C++ объектами и их деструкторами в нашей функции,
            // если вызывающий longjmp() код их не использует.
            return false;
        }

        png_init_io(png, aFile);

        png_set_IHDR(png, info, aImage.width(), aImage.height(),
            8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE,
            PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

        png_write_info(png, info);
        png_write_image(png, (png_bytep*)&row_pointers.front());
        png_write_end(png, info);

        return true;
    }

#if defined(__linux__) || defined (__APPLE__)
    FILE *reopenWriteBinary(FILE * aStream)
    {
        return fdopen(dup(fileno(stdout)), "wb");
    }

#elif defined(_WIN32)

#endif

}