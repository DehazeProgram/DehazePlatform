#include <iostream>
#include <opencv2/opencv.hpp>
#include "darkchanneldehazor.h"
#include "slicbasedehaze.h"
#include "slic.h"

using namespace std;

int main(int argc,char **argv)
{
    SlicBaseDehaze dehaze(std::string(argv[1]),std::string(argv[2]),150,1,0.01,220);
    dehaze.process();

    return 0;
}

