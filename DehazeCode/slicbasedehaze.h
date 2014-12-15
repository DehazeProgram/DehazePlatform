#ifndef SLICBASEDEHAZE_H
#define SLICBASEDEHAZE_H

#include "dehazecode_global.h"
#include "gdal_priv.h"
#include "slic.h"
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
    SlicBaseDehaze(std::string &imagePath, std::string &segPath, double m, float eps, float t, uchar max_a);
    void Process();
private:
    void Segmentation(const char* input, const char* output, int r, double m);
    void GenerateDarkImage();
    void GenerateAtmosphericRadiation();
    void GenereteTransmmision();
    void GenerateDehazeImage();
    void Init();

    std::string _imagePath;
    std::string _segPath;
    int _r;
    double _m;

    uchar _max_A;
    float _eps;
    float _t0;
    ARadiation_SB _A;

    cv::Mat _rawImage;
    cv::Mat _darkChannelImage;
    cv::Mat _transmission;
    cv::Mat _dehazeImage;
    std::vector<cv::Mat > _channelLayers;
};

#endif // SLICBASEDEHAZE_H
