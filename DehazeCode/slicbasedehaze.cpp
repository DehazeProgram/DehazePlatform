#include "slicbasedehaze.h"

SlicBaseDehaze::SlicBaseDehaze()
{
}

void SlicBaseDehaze::Segmentation(const char *input, const char *output, int r, double m)
{
    clock_t t1 = clock();

    GDALAllRegister();
    CPLSetConfigOption("GDAL_FILENAME_IS_UTF8","NO");
    GDALDataset* poSrcDS = (GDALDataset*) GDALOpen(input,GA_ReadOnly);
    if(poSrcDS == NULL)
    {
        std::cerr<<"Open File "<<input<<" Failed!"<<std::endl;
        return ;
    }
    GDALDriver* poDriver = (GDALDriver*)GDALGetDriverByName("GTiff");
    GDALDataset* poDstDS =  poDriver->Create(output,poSrcDS->GetRasterXSize(),poSrcDS->GetRasterYSize(),1,GDT_Int32,NULL);
    if (poDstDS == NULL)
    {
        std::cerr<<"Create File "<<output<<" Failed!"<<std::endl;
        return ;
    }
    poDstDS->SetProjection(poSrcDS->GetProjectionRef());
    double adfGeoTransform[6];
    poSrcDS->GetGeoTransform(adfGeoTransform);
    poDstDS->SetGeoTransform(adfGeoTransform);

    Slic slic(poSrcDS,poDstDS,r,m);
    slic.StartSegmentation();

    GDALClose(poSrcDS);
    GDALClose(poDstDS);


    std::cout<<"cost "<<static_cast<double>(clock()-t1)/CLOCKS_PER_SEC <<"s"<<std::endl;
}
