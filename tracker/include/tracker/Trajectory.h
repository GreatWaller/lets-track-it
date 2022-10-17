#pragma once

#include "common.h"
#include "Box.h"
#include "FixedQueue.hpp"

namespace gryllidae
{
    class Trajectory
    {
    private:
        FixedQueue<BBox,16> mTrajectory;
        // id is tracking id
        long mId;
        int mClassId;
        int mMaxLength;

        // should erase if not detected by detector for too long 
        long mLastDetected;
    public:
        
    public:
        Trajectory(BBox &bbox):mClassId(bbox.ClassId)
        {
            static long i=0;
            mId=i++;
            bbox.TrackId=mId;
            mLastDetected=bbox.PFrame->Id;
            Push(bbox);
            std::cout << "######################### Trajectory id: "<<mId<<"\n";
        }
        void Push(BBox bbox){
            mTrajectory.push(bbox);
        }
        inline bool IsSameClass(int classId){ return mClassId== classId;}

        BBox LastBox(){return mTrajectory.back();}
        inline void SetId(unsigned int id){
            mId=id;
            }
        inline unsigned int ID(){return mId;}

        inline void SetLastDetected(long frameId){mLastDetected=frameId;}
        inline unsigned long LastDetected(){return mLastDetected;}

    };

} // namespace gryllidae
