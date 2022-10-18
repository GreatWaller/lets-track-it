#pragma once

#include "common.h"
#include "Box.h"
#include "video/Frame.h"
#include "Event.h"

namespace gryllidae
{
    class Detector
    {
    public:
        using EventCallbackFn=std::function<void(Event&)>;

        virtual std::vector<BBox> Detect(Frame::Ptr frame) = 0;
        virtual ~Detector()=default;

        virtual void SetEventCallback(const EventCallbackFn& callback)=0;

    private:
        /* data */
        
    };

} // namespace gryllidae
