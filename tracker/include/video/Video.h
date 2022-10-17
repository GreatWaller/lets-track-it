#pragma once

#include <common.h>
#include <Frame.h>
#include <opencv2/videoio.hpp>

namespace gryllidae
{
    class Video
    {

    public:
        virtual std::shared_ptr<Frame> NextFrame() = 0;
        virtual int Width() const =0;
        virtual int Height() const =0;
    };
} // namespace gryllidae
