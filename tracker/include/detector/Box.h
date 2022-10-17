#pragma once
#include "Frame.h"

namespace gryllidae
{
    struct Box
    {
        float x,y,w,h;
    };

    struct BBox
    {
        using TBox = gryllidae::Box;
        TBox Box;
        int ClassId;
        float Probability;
        unsigned int TrackId;

        using FramePtr=gryllidae::Frame::Ptr;
        FramePtr PFrame=nullptr;
        std::vector<cv::Point2f> KeyPoints;
        int FeatureSize;
    };
    
    
} // namespace gryllidae

