#ifndef DARKIMAGEDEHAZOR_H
#define DARKIMAGEDEHAZOR_H
#include <opencv2/opencv.hpp>
#include <iostream>
#include "dehazecode_global.h"
#include"filter.h"

struct ARadiation_DC
{
    int r;
    int g;
    int b;
};

class DARKCHANNELDEHAZOR_SHARED_EXPORT DarkChannelDehazor
{
public:
    DarkChannelDehazor(QString &imagePath, float eps, float t, int max_a);
    DarkChannelDehazor(const cv::Mat &img, float eps, float t, int max_a);
    inline int getMinFliterWindowSize(){ return _minFliterWindowSize;}
    inline int getGuideFliterWindoeSize(){ return _guideFliterWindowSize;}
    inline int getMaxAtmosphericRadiation() {return _max_A;}

    inline void setMinFliterWindowSize(int s){ _minFliterWindowSize =s;}
    inline void setGuideFliterWindowSize(int s){ _guideFliterWindowSize =s;}
    inline void setMaxAtmosphericRadiation(int a) {  _max_A =a;}

    void Process(cv::Mat &dehazeImage, cv::Mat &DCImage, cv::Mat &transmissionImage);
    void Process(cv::Mat &dehazeImage);

private:
    cv::Mat rawImage;
    cv::Mat darkChannelImage;
    cv::Mat transmission;
    std::vector<cv::Mat > channelLayers;

    int _minFliterWindowSize;
    int _guideFliterWindowSize;
    float _max_A;
    float _eps;
    float _t0;
    ARadiation_DC _A;
    bool _optimizedcontrast;
//    Filter filter;

    void Init();

    cv::Mat &GenerateDarkImage();
    void GenerateAtmosphericRadiation();
    cv::Mat &GenereteTransmmision();
    void GenerateDehazeImage(cv::Mat &dehazeImage);
};

#endif // DARKIMAGEDEHAZOR_H
