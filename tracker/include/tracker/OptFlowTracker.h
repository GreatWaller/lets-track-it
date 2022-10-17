#pragma once

#include "common.h"
#include <opencv2/opencv.hpp>
#include "video/Frame.h"
#include "detector/Box.h"
#include "tracker/KeyPoint.h"
#include <map>
#include "Trajectory.h"

namespace gryllidae
{
    class OptFlowTracker
    {
    private:
        cv::Ptr<cv::SparsePyrLKOpticalFlow> pOptFlow;
        cv::Ptr<cv::GFTTDetector> pDetector;

        std::vector<cv::Point2f> mPrevFeatures;

        // cache
        cv::Mat mPrevGray;
        std::vector<BBox> mPrevBBox;

        std::vector<uchar> mStatus;
        std::vector<float> mError;
        // bool bIsInitialized=false;

        // tracking info
        // key is for class
        // std::map<int,int> mTrackingIdMap;
        std::list<std::shared_ptr<Trajectory>> mTrajectoryList;

        // config
        float mMaxDistance = 100;
        float mMinMatchingFeatures = 32;
        int mMaxMiss = 8;

    public:
        OptFlowTracker(int maxDistance = 100,
                       int minMatchingFeatures = 32,
                       int maxMiss = 8);
        // for test
        std::vector<cv::Point2f> Track(Frame::Ptr currentFrame, std::vector<BBox> bboxes);

        std::vector<BBox> Tracking(Frame::Ptr currentFrame, std::vector<BBox> bboxes);
    private:
        std::vector<cv::Point2f> DropOutliers(std::vector<cv::Point2f> points);
        BBox BoundingBox(std::vector<cv::Point2f> points);
        float Distance(const Box &last, const Box &current);
        int TrackingEntry(Frame::Ptr currentFrame, std::vector<BBox> bboxes);
        int TrackingInternal(Frame::Ptr currentFrame);
        void CleanTrajectories(Frame::Ptr currentFrame);
    };
} // namespace gryllidae
