#pragma once

#include "imaging.h"
#include <stdio.h>

/** Этот код участвует в компиляции только если выбрана опция USE_LIBPNG
*/
namespace gio
{

    /** Сохранение изображения в формате PNG.
    Почему FILE* а не имя файла или std::ostream? LibPNG требует FILE * или набор указателей на функции,
    и хотелось также оставить возможность вывода в stdout для просмотра в ImageMagick
    */
    bool pngWrite(imaging::Image<imaging::PixelRGBA> const & aImage, FILE * aFile);

    FILE *reopenWriteBinary(FILE * aStream);
}
