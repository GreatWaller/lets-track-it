#include <video/OpenCVVideo.h>
#include <iostream>

namespace gryllidae
{
    OpenCVVideo::OpenCVVideo(const std::string& url, int w, int h):
        mVideoCapture(url), width(w), height(h){

            if (!mVideoCapture.isOpened())
            {
                std::cerr<<"ERROR! Unable to open camera\n";
                
            }
            

        }
    
    std::shared_ptr<Frame> OpenCVVideo::NextFrame(){
        auto frame=std::make_shared<Frame>();
        mVideoCapture.read(frame->Image);

        return frame;
    }

} // namespace gryllidae
