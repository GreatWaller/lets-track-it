#pragma once
#include <common.h>
#include <opencv2/opencv.hpp>

namespace gryllidae
{
    struct Frame
    {
    public:
        using Ptr=std::shared_ptr<Frame>;
        unsigned long Id;
        cv::Mat Image;
        cv::Mat GrayImage;
        std::vector<cv::Point2f> KeyPoints;

        public:
        Frame();
        void Save();
        

    };
} // namespace gryllidae
