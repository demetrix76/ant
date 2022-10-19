#pragma once

#include "tracker.h"
#include <type_traits>
#include <deque>

namespace Ant
{

    /** Простая, но не рекурсивная реализация алгоритма Flood Fill. 
     * Потребляет достаточно много памяти, но нет риска переполнения стека.
     * @param aPoint стартовая точка
     * @param aCondition функция (int x, int y) => bool, определающая доступность точки
     * @param aTracker объект, отслеживающий посещённые точки.
     * Количество достижимых точек сохраняется в aTracker.
    */

    template<
        typename condition_t,
        typename _ = std::invoke_result_t<condition_t, int, int>
    >
    void visitPointQueue(Point const & aPoint, condition_t && aCondition, Tracker & aTracker)
    {
        std::deque<Point> queue;
        queue.push_back(aPoint);

        while(!queue.empty())
        {
            Point pt = queue.front();
            queue.pop_front();

            if(aCondition(pt.x, pt.y) && !aTracker.visited(pt))
            {
                aTracker.visit(pt);
                queue.push_back(pt.left());
                queue.push_back(pt.right());
                queue.push_back(pt.up());
                queue.push_back(pt.down());
            }
        }
    }


}