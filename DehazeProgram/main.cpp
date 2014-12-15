#include <iostream>
#include <opencv2/opencv.hpp>
#include "darkchanneldehazor.h"
#include "slicbasedehaze.h"
#include "slic.h"

using namespace std;
//"C:\\hr\\experiment\\images\\1.bmp" "C:\\hr\\experiment\\images\\sgm.tif"
int main(int argc,char **argv)
{
    SlicBaseDehaze dehaze =SlicBaseDehaze();
    dehaze.Segmentation(argv[1],argv[2],30,100.0);

    return 0;
}

