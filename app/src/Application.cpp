#include "Application.h"
#include "video/OpenCVVideo.h"
#include <iostream>
#include "Config.h"

using namespace gryllidae;

Application::Application()
{
    if (Config::SetConfigFile("../app/config/config.yaml") == false)
    {
        std::cerr << "Error: config fild not found.\n";
    }
    // pVideo.reset(new gryllidae::OpenCVVideo("rtsp://admin:fnst1234@10.10.20.67"));
    pVideo.reset(new gryllidae::OpenCVVideo(Config::Get<std::string>("input"),
                                            Config::Get<int>("width"),
                                            Config::Get<int>("height")));

    cv::namedWindow(cWindowName, cv::WINDOW_NORMAL);
    cv::resizeWindow(cWindowName, 800, 640);

    pDetector = std::move(DetectorCreator::Create(DetectorType::Yolo));
    mTargetClass = Config::Get<int>("target_class");

    pTracker.reset(new OptFlowTracker(Config::Get<int>("max_distance"),
                                      Config::Get<int>("min_matching_features"),
                                      Config::Get<int>("max_miss")));
    mSkip = Config::Get<int>("skip_frame");
}

bool Application::Step()
{
    auto frame = pVideo->NextFrame();

    if (frame->Image.empty())
    {
        std::cerr << "ERROR! blank frame grabbed\n";
        return false;
    }
    pFrame.swap(frame);
    // pFrame->Save();

    // detect and track

    std::vector<BBox> boxes;
    if (pFrame->Id % mSkip == 0)
    // if (pFrame->Id == 8)
    {
        boxes = pDetector->Detect(pFrame);
    }
    // filter
    if (mTargetClass >= 0)
    {
        std::vector<BBox> filteredBoxes;
        for (auto &&box : boxes)
        {
            if (box.ClassId == mTargetClass)
            {
                filteredBoxes.emplace_back(std::move(box));
            }
        }
        boxes = filteredBoxes;
    }

    // test tracker
    // auto points = pTracker->Track(pFrame, boxes);
    // Show(pFrame->Image, boxes, points);

    auto predictBBoxes = pTracker->Tracking(pFrame, boxes);

    DrawBoxAndPoints(pFrame->Image, boxes, predictBBoxes);

    return true;
}

void Application::Run()
{
    int i = 0;
    while (true)
    {
        std::cout << "Step " << i << ".\n";
        i++;
        if (!Step())
        {
            std::cerr << "Step Fail.\n";
            break;
        }
    }
}

void Application::Show(cv::Mat &image, std::vector<BBox> &bboxes,
                       std::vector<cv::Point2f> features) const
{

    // box
    // int width = pVideo->Width();
    // int height = pVideo->Height();

    for (auto &&bbox : bboxes)
    {
        // cv::rectangle(image,
        //               cv::Point2f((box.x - box.w / 2) * width, (box.y - box.h / 2) * height),
        //               cv::Point2f((box.x + box.w / 2) * width, (box.y + box.h / 2) * height),
        //               cv::Scalar(0, 0, 255), 3, 1, 0);
        cv::rectangle(image,
                      cv::Point2f(bbox.Box.x - bbox.Box.w, bbox.Box.y - bbox.Box.h / 2),
                      cv::Point2i(bbox.Box.x + bbox.Box.w / 2, bbox.Box.y + bbox.Box.h / 2),
                      cv::Scalar(0, 0, 255), 3, 1, 0);

        // std::cout<<"Box...\n";
    }

    for (auto &&p : features)
    {
        cv::circle(image, p, 1, cv::Scalar(0, 0, 255), 2);
        // cv::line(image,)
    }
    cv::Rect rect = cv::boundingRect(features);
    cv::rectangle(image, rect, cv::Scalar(0, 255, 0), 3, 1, 0);

    cv::imshow(cWindowName, image);
    cv::waitKey(1);
}

void Application::DrawBoxAndPoints(cv::Mat &image,
                                   std::vector<gryllidae::BBox> &detectedBoxes,
                                   std::vector<gryllidae::BBox> &trackingBoxes)
{
    for (auto &&bbox : detectedBoxes)
    {
        cv::rectangle(image,
                      cv::Point2f(bbox.Box.x - bbox.Box.w, bbox.Box.y - bbox.Box.h / 2),
                      cv::Point2f(bbox.Box.x + bbox.Box.w / 2, bbox.Box.y + bbox.Box.h / 2),
                      cv::Scalar(0, 0, 255), 3, 1, 0);
    }

    for (auto &&bbox : trackingBoxes)
    {
        cv::rectangle(image,
                      cv::Point2f(bbox.Box.x - bbox.Box.w, bbox.Box.y - bbox.Box.h / 2) + cv::Point2f(5, 5),
                      cv::Point2f(bbox.Box.x + bbox.Box.w / 2, bbox.Box.y + bbox.Box.h / 2) - cv::Point2f(5, 5),
                      cv::Scalar(0, 255, 0), 3, 1, 0);
        for (auto &&p : bbox.KeyPoints)
        {
            cv::circle(image, p, 1, cv::Scalar(0, 0, 255), 2);
        }
    }
    cv::imshow(cWindowName, image);
    cv::waitKey(1);
}
