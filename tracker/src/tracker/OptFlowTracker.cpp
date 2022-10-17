#include "tracker/OptFlowTracker.h"

namespace gryllidae
{
    OptFlowTracker::OptFlowTracker(int maxDistance,
                                   int minMatchingFeatures,
                                   int maxMiss)
        : mMaxDistance(maxDistance),
          mMinMatchingFeatures(minMatchingFeatures),
          mMaxMiss(maxMiss)
    {
        // TODO: init parameters
        pOptFlow = cv::SparsePyrLKOpticalFlow::create();

        pDetector = cv::GFTTDetector::create(
            64, 0.01, 20);
    }

    std::vector<cv::Point2f> OptFlowTracker::Track(Frame::Ptr currentFrame,
                                                   std::vector<BBox> bboxes)
    {
        cv::Mat currentGray;
        cv::cvtColor(currentFrame->Image, currentGray, cv::COLOR_RGB2GRAY);
        // TODO: detect feature if having a bbox?

        std::vector<cv::Point2f> currentPoints;
        std::vector<cv::Point2f> goodPoints;

        if (mPrevFeatures.size() <= 48)
        {
            std::vector<cv::KeyPoint> currentFeatures;
            // mask
            cv::Mat mask(currentFrame->Image.size(), CV_8UC1, 255);

            for (auto &bbox : bboxes)
            {
                cv::rectangle(mask,
                              cv::Point2f(bbox.Box.x - bbox.Box.w / 2, bbox.Box.y - bbox.Box.h / 2) + cv::Point2f(10, 10),
                              cv::Point2f(bbox.Box.x + bbox.Box.w / 2, bbox.Box.y + bbox.Box.h / 2) - cv::Point2f(10, 10),
                              0, -1);
            }
            if (bboxes.size() <= 0)
            {
                for (auto &bbox : mPrevBBox)
                {
                    cv::rectangle(mask,
                                  cv::Point2f(bbox.Box.x - bbox.Box.w / 2, bbox.Box.y - bbox.Box.h / 2) + cv::Point2f(10, 10),
                                  cv::Point2f(bbox.Box.x + bbox.Box.w / 2, bbox.Box.y + bbox.Box.h / 2) - cv::Point2f(10, 10),
                                  0, -1);
                }

                // std::cout << "### Previous BBox: " << mPrevBBox[0].Box.x << " " << mPrevBBox[0].Box.y << "\n";
            }

            mask = 255 - mask;
            pDetector->detect(currentGray, currentFeatures, mask);
            for (auto &&kp : currentFeatures)
            {
                goodPoints.push_back(kp.pt);
            }
        }
        else
        {
            // track
            pOptFlow->calc(mPrevGray, currentGray, mPrevFeatures, currentPoints, mStatus, mError);

            for (size_t i = 0; i < mStatus.size(); i++)
            {
                if (mStatus[i])
                {
                    goodPoints.push_back(currentPoints[i]);
                }
            }
        }

        // drop outliers
        goodPoints = DropOutliers(goodPoints);

        mPrevGray = currentGray;
        mPrevFeatures = goodPoints;
        std::cout << "Good Points: " << goodPoints.size() << "\n";

        // update bbox
        auto bbox = BoundingBox(goodPoints);
        mPrevBBox = {bbox};

        return goodPoints;
    }

    // track and save trajectory
    int OptFlowTracker::TrackingInternal(Frame::Ptr currentFrame)
    {
        // track
        // all trajectories need to match its key points
        if (mTrajectoryList.size() == 0)
        {
            return 0;
        }
        cv::cvtColor(currentFrame->Image, currentFrame->GrayImage, cv::COLOR_RGB2GRAY);

        int res = 0;
        for (auto &&traj : mTrajectoryList)
        {

            BBox lastBox = traj->LastBox();
            if (lastBox.PFrame == nullptr)
            {
                continue;
            }
            else
            {
                // gray image
                if (lastBox.PFrame->GrayImage.empty())
                {
                    cv::cvtColor(currentFrame->Image, lastBox.PFrame->GrayImage,
                                 cv::COLOR_RGB2GRAY);
                }

                // features in box .
                // make sure features > 0
                if (lastBox.FeatureSize <= 0)
                {
                    std::cout << " Tracking Error: Last box has no key points.\n";
                    return 0;
                }
                // opt

                std::vector<cv::Point2f> currentPoints;
                std::vector<cv::Point2f> goodPoints;
                pOptFlow->calc(lastBox.PFrame->GrayImage,
                               currentFrame->GrayImage,
                               lastBox.KeyPoints, currentPoints,
                               mStatus, mError);

                for (size_t i = 0; i < mStatus.size(); i++)
                {
                    if (mStatus[i])
                    {
                        goodPoints.push_back(currentPoints[i]);
                    }
                }
                // drop outliers
                goodPoints = DropOutliers(goodPoints);
                std::cout << "Matching Points: " << goodPoints.size() << "\n";

                // need to delete
                for (auto &&p : goodPoints)
                {
                    currentFrame->KeyPoints.push_back(p);
                }

                if (goodPoints.size() > mMinMatchingFeatures)
                {
                    // matched. push to its trajectory

                    BBox newBBox = BoundingBox(goodPoints);
                    newBBox.ClassId = lastBox.ClassId;
                    newBBox.FeatureSize = goodPoints.size();
                    newBBox.KeyPoints = goodPoints;
                    newBBox.PFrame = currentFrame;
                    newBBox.Probability = lastBox.Probability;
                    newBBox.TrackId = lastBox.TrackId;

                    traj->Push(newBBox);
                    res++;
                }
                else
                {
                    std::cout << "Object Disappeared.\n";
                }
            }
        }
        return 0;
    }

    // try to set tracking id of each bbox

    int OptFlowTracker::TrackingEntry(Frame::Ptr currentFrame,
                                 std::vector<BBox> bboxes)
    {
        CleanTrajectories(currentFrame);

        // this function is to tracking current bbox
        if (bboxes.size() <= 0)
        {
            // opt tracking
            int num = TrackingInternal(currentFrame);
            return num;
        }

        // detector new features
        cv::cvtColor(currentFrame->Image, currentFrame->GrayImage, cv::COLOR_RGB2GRAY);

        for (auto &bbox : bboxes)
        {
            std::vector<cv::KeyPoint> currentFeatures;
            // mask
            cv::Mat mask(currentFrame->Image.size(), CV_8UC1, 255);
            cv::rectangle(mask,
                          cv::Point2f(bbox.Box.x - bbox.Box.w / 2, bbox.Box.y - bbox.Box.h / 2) + cv::Point2f(20, 20),
                          cv::Point2f(bbox.Box.x + bbox.Box.w / 2, bbox.Box.y + bbox.Box.h / 2) - cv::Point2f(20, 20),
                          0, -1);
            mask = 255 - mask;
            pDetector->detect(currentFrame->GrayImage, currentFeatures, mask);
            for (auto &&feature : currentFeatures)
            {
                bbox.KeyPoints.push_back(feature.pt);
                // TODO: delete
                currentFrame->KeyPoints.push_back(feature.pt);
            }
            bbox.FeatureSize = currentFeatures.size();
            bbox.PFrame = currentFrame;
        }

        //
        if (mTrajectoryList.size() == 0)
        {
            for (auto &&bbox : bboxes)
            {
                // new object
                auto traj = std::make_shared<Trajectory>(bbox);
                mTrajectoryList.push_back(traj);
            }
            return 0;
        }

        else
        {
            int trackCount = 0;
            std::vector<float> distVec(bboxes.size(),
                                       std::numeric_limits<float>::max());
            for (size_t i = 0; i < bboxes.size(); i++)
            {
                auto bbox = bboxes[i];
                // int indexTraj = -1;
                std::shared_ptr<Trajectory> nearest = nullptr;
                for (auto &&traj : mTrajectoryList)
                {
                    if (!traj->IsSameClass(bbox.ClassId))
                    {
                        continue;
                    }
                    // find same object
                    auto lastBox = traj->LastBox();
                    auto d = Distance(lastBox.Box, bbox.Box);
                    if (d < distVec[i])
                    {
                        distVec[i] = d;
                        // indexTraj = t;
                        nearest = traj;
                    }
                }
                if (distVec[i] < mMaxDistance)
                {
                    bbox.TrackId = nearest->ID();
                    nearest->Push(bbox);
                    nearest->SetLastDetected(currentFrame->Id);
                    trackCount++;
                }
                else
                {
                    // bbox.TrackId = ++mTrackingIdMap[bbox.ClassId];
                    // new traj
                    auto traj = std::make_shared<Trajectory>(bbox);
                    mTrajectoryList.push_back(traj);
                }
            }
            return trackCount;
        }
    }

    std::vector<BBox> OptFlowTracker::Tracking(Frame::Ptr currentFrame, std::vector<BBox> bboxes){
        auto num=TrackingEntry(currentFrame,bboxes);
        std::vector<BBox> result;
        for (auto &&traj : mTrajectoryList)
        {
            result.push_back(traj->LastBox());
        }
        return result;
    }


    std::vector<cv::Point2f> OptFlowTracker::DropOutliers(std::vector<cv::Point2f> points)
    {
        if (points.size() <= 0)
        {
            return std::vector<cv::Point2f>{};
        }
        int outliers;
        // center
        cv::Moments m = cv::moments(points, true);
        cv::Point2f centroid(m.m10 / m.m00, m.m01 / m.m00);

        // distance
        std::vector<double> distances;
        for (auto &p : points)
        {
            double d = std::sqrt((p.x - centroid.x) * (p.x - centroid.x) +
                                 (p.y - centroid.y) * (p.y - centroid.y));
            // maybe x+y;
            // double d=(p.x-centroid.x)+(p.y-centroid.y);
            distances.push_back(d);
        }

        cv::Scalar mu, sigma;
        cv::meanStdDev(distances, mu, sigma);

        double thresh = mu.val[0] + 2.0 * sigma.val[0];

        std::vector<cv::Point2f> inliers;
        for (size_t i = 0; i < distances.size(); i++)
        {
            if (distances[i] > thresh)
            {
                outliers++;
                continue;
            }
            inliers.push_back(points[i]);
        }
        return inliers;
    }

    BBox OptFlowTracker::BoundingBox(std::vector<cv::Point2f> points)
    {
        cv::Rect2f rect = cv::boundingRect(points);
        return BBox{Box{rect.x + rect.width / 2,
                        rect.y + rect.height / 2,
                        rect.width,
                        rect.height}};
    }

    float OptFlowTracker::Distance(const Box &box, const Box &otherOne)
    {
        // upper center
        float xD = box.x - otherOne.x;
        float yD = (box.y - box.h / 2) - (otherOne.y - otherOne.h / 2);

        float d = std::sqrt(xD * xD + yD * yD);
        return d;
    }

    void OptFlowTracker::CleanTrajectories(Frame::Ptr currentFrame)
    {
        for (auto it = mTrajectoryList.begin(); it != mTrajectoryList.end();)
        {
            auto lastBox = (*it)->LastBox();
            // if (currentFrame->Id - lastBox.PFrame->Id > mMaxMiss)
            if (static_cast<int>(currentFrame->Id - lastBox.PFrame->Id) > mMaxMiss || static_cast<int>(currentFrame->Id - (*it)->LastDetected()) > mMaxMiss)
            {
                // remove
                it = mTrajectoryList.erase(it);
            }
            else
            {
                it++;
            }
        }
    }

} // namespace gryllidae
