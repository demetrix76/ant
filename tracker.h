#pragma once

#include "point.h"
#include "imaging.h"
#include <unordered_set>

namespace Ant
{
    /** Абстрактный интерфейс для трекера посещённых точек. */
    class Tracker
    {
    public:
        virtual ~Tracker() {}

        /// true если точка уже посещалась
        virtual bool visited(Point const &) const = 0;

        /// отметить точку как посещённую
        virtual void visit(Point const &) = 0;

        /// количество посещённых точек
        virtual std::size_t size() const = 0;

        /// закрасить посещённые точки на изображении
        virtual void overlay(imaging::ImageRGBA &) const = 0;

    };

    /** Трекер посещённых точек на основе std::unordered_set.    
     *  Возможны различные реализации, и без тестирования сложно сказать, какая будет наиболее эффективной.
     *  Можно использовать динамически растущую битовую карту,
     *  однако по условиям задачи мы рассматриваем бесконечную координатную сетку.
     *  В таком случае безопаснее хранить список посещённых точек, по возможности
     *  обеспечив максимально эффективный поиск в этом списке.
     *  HashSetTracker использует std::unordered_set для эффективного поиска.
    */
    class HashSetTracker : public Tracker
    {
    public:
        bool visited(Point const &) const override;

        void visit(Point const &) override;

        std::size_t size() const override;

        void overlay(imaging::ImageRGBA &) const override;

    private:
        std::unordered_set<Point>   mVisitedPoints;
    };

}
