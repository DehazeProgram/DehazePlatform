#ifndef SLICBASEDEHAZE_H
#define SLICBASEDEHAZE_H

#include "slic.h"
#include "gdal_priv.h"
#include "dehazecode_global.h"

struct ARadiation_SB
{
    int r;
    int g;
    int b;
};

class SLICBASEDEHAZE_SHARED_EXPORT SlicBaseDehaze
{
public:
    SlicBaseDehaze(std::string &imagePath,std::string &segmentationPath,int regionSize,double regularizer,
                    float eps, float t, int max_a);
private:
    void SlicSegmentation(std::string &imagePath,std::string &segmentationPath,int regionSize,double regularizer);
};

#endif // SLICBASEDEHAZE_H
