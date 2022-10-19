#include "tracker.h"
#include <iostream>
#include <algorithm>

namespace Ant
{

    // ============================================================
    // HashSetTracker
    // ============================================================

    bool HashSetTracker::visited(Point const & aPoint) const
    {
        return mVisitedPoints.count(aPoint) > 0;
    }

    void HashSetTracker::visit(Point const & aPoint)
    {
        mVisitedPoints.insert(aPoint);
    }

    std::size_t HashSetTracker::size() const
    {
        return mVisitedPoints.size();
    }

    void HashSetTracker::overlay(imaging::ImageRGBA & aImage) const
    {
        for(Point const & pt: mVisitedPoints)
        {
            if(pt.x >= 0 && pt.y >= 0 && pt.x < aImage.width() && pt.y < aImage.height())
            {
                imaging::PixelRGBA & pixel = aImage.pixel(pt.x, pt.y);
                pixel = (pixel == imaging::PixelWHITE ? imaging::PixelGREEN : imaging::PixelRED);
            }
        }
    }

}