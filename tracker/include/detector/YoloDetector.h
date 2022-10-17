#pragma once

#include "common.h"
#include "Detector.h"
#include "darknet.h"

namespace gryllidae
{
    class YoloDetector:public Detector
    {
    public:
        // using NetWork=::network;
    private:
        std::string mWeights;
        std::string mCfg;
        float mThresh;
        float mNms;

        network *mNet;
        int w,h;
        int classes;

    public:
        YoloDetector(const std::string& weights, const std::string &cfg);

        ~YoloDetector();
        std::vector<BBox> Detect(Frame::Ptr frame) override;

    private:
        image MatToImage(cv::Mat mat);
        int max_index(float *a, int n);
    };
} // namespace gryllidae
