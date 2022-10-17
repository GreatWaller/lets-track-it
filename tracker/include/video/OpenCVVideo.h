#pragma once

#include <common.h>
#include <Video.h>
#include <opencv2/videoio.hpp>

namespace gryllidae
{
    class OpenCVVideo : public Video
    {
    private:
        cv::VideoCapture mVideoCapture;
        int width, height;

    public:
        OpenCVVideo(const std::string &url, int w=1920, int h=1080);

        std::shared_ptr<Frame> NextFrame() override;

        inline int Width() const override {return width;} 
        inline int Height() const override { return height;}
    };
} // namespace gryllidae