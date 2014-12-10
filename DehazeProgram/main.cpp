#include <iostream>
#include <opencv2/opencv.hpp>
#include "darkchanneldehazor.h"

using namespace std;

int main(int argc,char **argv)
{
    cv::Mat hazeMat = cv::imread(std::string(argv[1]));
    cv::Mat dehazeMat;
    DarkChannelDehazor dehazor(hazeMat,1,0.01,220);
    dehazor.Process(dehazeMat);
    cv::imshow("dehaze image",dehazeMat);
    cv::waitKey();

    return 0;
}

