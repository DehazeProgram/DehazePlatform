#ifndef SLICBASEDEHAZE_H
#define SLICBASEDEHAZE_H

#include "dehazecode_global.h"
#include "gdal_priv.h"
#include "slic.h"

class SLICBASEDEHAZE_SHARED_EXPORT SlicBaseDehaze
{
public:
    SlicBaseDehaze();

    void Segmentation(const char* input, const char* output, int r, double m);
};

#endif // SLICBASEDEHAZE_H
