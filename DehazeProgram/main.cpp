#include <iostream>
#include <opencv2/opencv.hpp>
#include "darkchanneldehazor.h"
#include "slicbasedehaze.h"
#include "slic.h"

using namespace std;
//"C:\\hr\\experiment\\images\\1.bmp" "C:\\hr\\experiment\\images\\sgm.tif"
int main(int argc,char **argv)
{
    SlicBaseDehaze dehaze =SlicBaseDehaze(std::string(argv[1]),std::string(argv[2]),100.0,1,0.01,220);
    dehaze.Process();

    return 0;
}

