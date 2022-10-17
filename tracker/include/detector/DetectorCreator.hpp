#pragma once

#include "common.h"
#include "Detector.h"
#include "FakeDetector.h"
#include "YoloDetector.h"

namespace gryllidae
{
    enum class DetectorType
    {
        Fake = 0,
        Yolo = 1
    };
    class DetectorCreator
    {
    private:
    public:
        static std::unique_ptr<Detector> Create(DetectorType detectorType)
        {
            switch (detectorType)
            {
            case DetectorType::Fake:
                return std::make_unique<FakeDetector>();
                // break;
            case DetectorType::Yolo:
                return std::make_unique<YoloDetector>(
                    "../model/yolov4.weights",
                    "../model/yolov4.cfg");
            default:
                return std::make_unique<FakeDetector>();
            }
        }
    };
} // namespace gryllidae
