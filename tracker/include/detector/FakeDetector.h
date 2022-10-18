#pragma once
#include "Detector.h"

namespace gryllidae
{
    class FakeDetector:public Detector
    {
    private:
        EventCallbackFn mEventCallback;
        
    public:
        FakeDetector();
        std::vector<BBox> Detect(Frame::Ptr frame) override;

        void SetEventCallback(const EventCallbackFn& callback) override{ mEventCallback=callback;}

    };
} // namespace gryllidae
