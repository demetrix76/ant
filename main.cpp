#include "traversal_stack.h"
#include "traversal_queue.h"
#include "gio.h"
#include "finally.h"

#include <iostream>
#include <cstdlib>
#include <utility>
#include <cstdlib>
#include <chrono>


/* Поскольку мы полагаемся на std::div и std::abs, имеет смысл обобщить функцию подсчета
   суммы цифр для всех типов, для которых определены эти библиотечные функции.
   Элегантнее было бы на C++20 с концептами, но пусть будет C++17 и SFINAE.
*/
template<typename T, 
    typename _t1 = decltype(std::div(std::declval<T>(), std::declval<T>())),
    typename retval_t = decltype(std::abs(std::declval<T>()))
    >
inline retval_t sumDigits(T aValue) 
{
    T sum {};
    while(aValue)
    {
        auto [quotient, remainder] = std::div(aValue, T{10});
        sum += remainder;
        aValue = quotient;
    }
    return std::abs(sum); // нас интересует сумма цифр без учёта знака
}

/* Данная задача решаеся классическим алгоритмом заливки замкнутой области цветом (FloodFill).
   Есть и существенные отличия:
   - заливка цветом по определению работает в конечном пространстве изображения,
        здесь же по условиям задачи мы работаем на бесконечной координатной сетке,
        границы заполняемой области определяются только условием (сумма цифр координат)
   - в случае с заливкой мы автоматически получаем отслеживание уже закрашенных точек,
        а в этой задаче нам необходим иной механизм отслеживания.
*/

int main(int, char*[])
{
    Ant::HashSetTracker tracker;

    auto condition = [](int x, int y) {
        return (sumDigits(x) + sumDigits(y)) <= 25;
    };

    auto start = std::chrono::steady_clock::now();
    Ant::visitPointQueue({1000, 1000}, condition, tracker);
    std::size_t cell_count = tracker.size();
    auto end = std::chrono::steady_clock::now();

    std::cerr << "Visited " << cell_count << " cells in "
        << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
        << " milliseconds" << std::endl;

#ifdef USE_LIBPNG
    imaging::ImageRGBA image(2000, 2000, [condition](int x, int y){
        return condition(x, y) ? imaging::PixelWHITE : imaging::PixelBLACK;
    });

    tracker.overlay(image);

    FILE *out_stream = gio::reopenWriteBinary(stdout);
    handy::finally fin_stream {[=]{
        fclose(out_stream);
    }};
    
    gio::pngWrite(image, out_stream);
#endif

    return 0;
}
