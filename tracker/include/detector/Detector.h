#pragma once

#include "common.h"
#include "Box.h"
#include "video/Frame.h"

namespace gryllidae
{
    class Detector
    {
    private:
        /* data */
    public:
        virtual std::vector<BBox> Detect(Frame::Ptr frame) = 0;
        virtual ~Detector(){}

        
    };

} // namespace gryllidae
