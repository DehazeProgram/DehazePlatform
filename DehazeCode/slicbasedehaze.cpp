#include "slicbasedehaze.h"
#include "slic.h"

SlicBaseDehaze::SlicBaseDehaze(std::string &imagePath, std::string &segmentationPath, int regionSize, double regularizer, float eps, float t, int max_a)
{

}

void SlicBaseDehaze::SlicSegmentation(std::string &imagePath, std::string &segmentationPath, int regionSize, double regularizer)
{
    clock_t t1 = clock();

    GDALAllRegister();
    CPLSetConfigOption("GDAL_FILENAME_IS_UTF8","NO");
    GDALDataset* poSrcDS = (GDALDataset*) GDALOpen(imagePath.c_str(),GA_ReadOnly);
    if(poSrcDS == NULL)
    {
        std::cerr<<"Open File "<<imagePath<<" Failed!"<<std::endl;
        return ;
    }
    GDALDriver* poDriver = (GDALDriver*)GDALGetDriverByName("GTiff");
    GDALDataset* poDstDS =  poDriver->Create(segmentationPath.c_str(),poSrcDS->GetRasterXSize(),poSrcDS->GetRasterYSize(),1,GDT_Int32,NULL);
    if (poDstDS == NULL)
    {
        std::cerr<<"Create File "<<segmentationPath<<" Failed!"<<std::endl;
        return ;
    }
    poDstDS->SetProjection(poSrcDS->GetProjectionRef());
    double adfGeoTransform[6];
    poSrcDS->GetGeoTransform(adfGeoTransform);
    poDstDS->SetGeoTransform(adfGeoTransform);

    Slic slic(poSrcDS,poDstDS,regionSize,regularizer);
    slic.StartSegmentation();

    GDALClose(poSrcDS);
    GDALClose(poDstDS);


    std::cout<<"cost "<<static_cast<double>(clock()-t1)/CLOCKS_PER_SEC <<"s"<<std::endl;
    return ;
}
