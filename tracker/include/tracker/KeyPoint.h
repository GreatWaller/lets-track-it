#pragma once

#include "common.h"

namespace gryllidae
{
    struct KeyPoint
    {
        cv::Point2f Point;
        double DistanceToCenter;
        bool IsOutlier;

        KeyPoint(cv::Point2f p):Point(p){}
    };
    
} // namespace gryllidae
