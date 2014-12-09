#ifndef DCDEHAZE_H
#define DCDEHAZE_H

#include <opencv2/opencv.hpp>
#include "dehaze.h"

class DCDehaze:public Dehaze
{
public:
    DCDehaze();
    ~DCDehaze();
    cv::Mat& GetDarkChannelImage() { return darkChannelImage;}
    cv::Mat& GetTransmissionImage() {return transmissionImage;}

private:
    cv::Mat darkChannelImage;
    cv::Mat transmissionImage;
};

#endif // DCDEHAZE_H
