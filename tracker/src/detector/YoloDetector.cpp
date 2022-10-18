#include "YoloDetector.h"
#include "BBoxDrawingEvent.h"

namespace gryllidae
{
    YoloDetector::YoloDetector(const std::string &weights, const std::string &cfg)
        : mWeights(weights), mCfg(cfg), mThresh(0.5f), mNms(0.45f)
    {
        mNet = load_network(const_cast<char *>(mCfg.c_str()),
                            const_cast<char *>(mWeights.c_str()), 0);

        w = (*mNet).w;
        h = (*mNet).h;
        layer l = (*mNet).layers[(*mNet).n - 1];
        classes = l.classes;
    }

    std::vector<BBox> YoloDetector::Detect(Frame::Ptr frame)
    {
        image im = MatToImage(frame->Image);
        image sized = resize_image(im, (*mNet).w, (*mNet).h);
        float *X = sized.data;
        network_predict(*mNet, X);
        int nboxes = 0;
        detection *dets = get_network_boxes(
            mNet, im.w, im.h, mThresh, 0.5, 0, 1, &nboxes, 0);
        free_image(im);
        free_image(sized);

        if (mNms > 0.f)
        {
            // if (l.nms_kind == DEFAULT_NMS)
            do_nms_sort(dets, nboxes, classes, mNms);
            // else
            //     diounms_sort(dets, nboxes, l.classes, mNms, l.nms_kind, l.beta_nms);
        }
        std::vector<BBox> bbox_vec;

        for (int i = 0; i < nboxes; ++i)
        {
            box b = dets[i].bbox;
            int const obj_id = max_index(dets[i].prob, classes);
            float const prob = dets[i].prob[obj_id];

            if (prob > mThresh)
            {
                BBox bbox;
                // bbox.x = std::max((double)0, (b.x - b.w / 2.)*im.w);
                // bbox.y = std::max((double)0, (b.y - b.h / 2.)*im.h);
                bbox.Box.x = b.x * im.w;
                bbox.Box.y = b.y * im.h;
                bbox.Box.w = b.w * im.w;
                bbox.Box.h = b.h * im.h;
                bbox.ClassId = obj_id;
                bbox.Probability = prob;
                bbox.TrackId = 0;
                // bbox.frames_counter = 0;
                bbox.PFrame=frame;

                bbox_vec.push_back(bbox);
            }
        }

        free_detections(dets, nboxes);

        // callback
        BBoxDrawingEvent e(bbox_vec);
        mEventCallback(e);
        return bbox_vec;
    }

    image YoloDetector::MatToImage(cv::Mat mat)
    {
        int w = mat.cols;
        int h = mat.rows;
        int c = mat.channels();
        image im = make_image(w, h, c);
        unsigned char *data = (unsigned char *)mat.data;
        int step = mat.step;
        for (int y = 0; y < h; ++y)
        {
            for (int k = 0; k < c; ++k)
            {
                for (int x = 0; x < w; ++x)
                {
                    im.data[k * w * h + y * w + x] = data[y * step + x * c + k] / 255.0f;
                }
            }
        }
        return im;
    }
    YoloDetector::~YoloDetector()
    {
        free_network(*mNet);
    }

    int YoloDetector::max_index(float *a, int n)
    {
        if (n <= 0)
            return -1;
        int i, max_i = 0;
        float max = a[0];
        for (i = 1; i < n; ++i)
        {
            if (a[i] > max)
            {
                max = a[i];
                max_i = i;
            }
        }
        return max_i;
    }


} // namespace gryllidae
