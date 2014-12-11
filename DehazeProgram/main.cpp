#include <iostream>
#include <opencv2/opencv.hpp>
#include "darkchanneldehazor.h"
#include "slicbasedehaze.h"
#include "slic.h"

using namespace std;

int main(int argc,char **argv)
{
    SlicBaseDehaze dehaze(string("C:\\hr\\experiment\\CDt\\0804_1w_mosaic_218.img"),string("C:\\hr\\experiment\\CDt\\0804_1w_mosaic_218.tif"),30,100,1,0.01,220);
    dehaze.SlicSegmentation(string("C:\\hr\\experiment\\CDt\\0804_1w_mosaic_218.img"),string("C:\\hr\\experiment\\CDt\\0804_1w_mosaic_218.tif"),30,200);

    return 0;
}

