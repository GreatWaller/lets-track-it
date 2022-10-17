#include "video/Frame.h"
namespace gryllidae
{

    void Frame::Save(){
        cv::imwrite("Camera.jpg",Image);
    }

    Frame::Frame(){
        static unsigned long i=0;
        std::cout<<"Frame ID: "<<i<<"\n";
        Id=i++;
    }
} // namespace gryllidae