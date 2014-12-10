#include "dcdehaze.h"

DCDehaze::DCDehaze()
{
}

DCDehaze::~DCDehaze()
{
    darkChannelImage.release();
    transmissionImage.release();
}


