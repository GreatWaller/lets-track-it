#pragma once
#include "Detector.h"

namespace gryllidae
{
    class FakeDetector:public Detector
    {
    private:
        
    public:
        FakeDetector();
        std::vector<BBox> Detect(Frame::Ptr frame) override;
    };
} // namespace gryllidae
