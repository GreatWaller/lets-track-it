#pragma once
#include "video/Video.h"
#include <opencv2/highgui.hpp>
#include "detector/Detector.h"
#include "DetectorCreator.hpp"
#include "tracker/OptFlowTracker.h"
#include "Event.h"

class Application
{

public:
    Application();
    void Run();
    bool Step();
    void Show(cv::Mat &image, std::vector<gryllidae::BBox> &boxes,
              std::vector<cv::Point2f> features) const;
    void DrawBoxAndPoints(cv::Mat &image,
                          std::vector<gryllidae::BBox> &detectedBoxes,
                          std::vector<gryllidae::BBox> &trackingBoxes);
    void OnEvent(gryllidae::Event& e);

private:
    std::unique_ptr<gryllidae::Video> pVideo = nullptr;
    std::shared_ptr<gryllidae::Frame> pFrame = nullptr;

    const std::string cWindowName{"Camera"};

    std::shared_ptr<gryllidae::Detector> pDetector = nullptr;
    std::shared_ptr<gryllidae::OptFlowTracker> pTracker=nullptr;

    int mSkip=8;

    // detector
    int mTargetClass=-1;
};