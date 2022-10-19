#pragma once

#include "tracker.h"
#include <type_traits>

namespace Ant
{

    /** Самая простая рекурсивная реализация алгоритма Flood Fill. 
     * Недостаток - большое потребление памяти с вероятностью переполнения стека,
     * но с данной задачей этот алгоритм справляется за достаточно короткое время.
     * @param aPoint стартовая точка
     * @param aCondition функция (int x, int y) => bool, определающая доступность точки
     * @param aTracker объект, отслеживающий посещённые точки.
     * Количество достижимых точек сохраняется в aTracker.
    */
    template<
        typename condition_t,
        typename _ = std::invoke_result_t<condition_t, int, int>
    >
    void visitPointSimple(Point const & aPoint, condition_t && aCondition, Tracker & aTracker)
    {
        if(aCondition(aPoint.x, aPoint.y) && !aTracker.visited(aPoint))
        {
            aTracker.visit(aPoint);
            visitPointSimple(aPoint.up(), std::forward<condition_t>(aCondition), aTracker);
            visitPointSimple(aPoint.down(), std::forward<condition_t>(aCondition), aTracker);
            visitPointSimple(aPoint.left(), std::forward<condition_t>(aCondition), aTracker);
            visitPointSimple(aPoint.right(), std::forward<condition_t>(aCondition), aTracker);
        }
    }

}