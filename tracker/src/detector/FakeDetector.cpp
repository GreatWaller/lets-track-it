#include "detector/FakeDetector.h"

namespace gryllidae
{
    FakeDetector::FakeDetector(){

    }


    std::vector<BBox> FakeDetector::Detect(Frame::Ptr frame) {
        // std::vector<Box> boxes{
        //     {0.2f,0.3f,0.1f,0.4f}
        // };
        static long id;
        if (id>8)
        {
            return std::vector<BBox>{};
        }
        
        std::vector<BBox> boxes{
            {{615.f,410.f,225.f,465.f},0,0.8f,0}
        };
        id++;
        return boxes;
    }
} // namespace gryllidae
