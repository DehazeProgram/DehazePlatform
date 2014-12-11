#ifndef SLICBASEDEHAZE_H
#define SLICBASEDEHAZE_H

#include "slic.h"
#include "gdal_priv.h"
#include "dehazecode_global.h"
#include "opencv2/opencv.hpp"

struct ARadiation_SB
{
    int r;
    int g;
    int b;
};

class SLICBASEDEHAZE_SHARED_EXPORT SlicBaseDehaze
{
public:
    SlicBaseDehaze(std::string &imagePath,std::string &segmentationPath,double regularizer,
                    float eps, float t, int max_a);
    void process();
private:

    cv::Mat rawImage;
    cv::Mat darkChannelImage;
    cv::Mat transmission;
    std::vector<cv::Mat > channelLayers;

    int _regionSize;
    int _guideFliterWindowSize;
    double _regularizer;
    float _max_A;
    float _eps;
    float _t0;
    std::string _segmentationPath;
    std::string _imagePath;
    ARadiation_SB _A;
    bool _optimizedcontrast;


    void Init();
    void SlicSegmentation();
    cv::Mat &GenerateDarkImage();
};

#endif // SLICBASEDEHAZE_H
