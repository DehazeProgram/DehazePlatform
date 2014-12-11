#include "slicbasedehaze.h"
#include "slic.h"

SlicBaseDehaze::SlicBaseDehaze(std::string &imagePath, std::string &segmentationPath, double regularizer, float eps, float t, int max_a)
:_eps(eps),_max_A(max_a),_t0(t),_segmentationPath(segmentationPath),_regularizer(regularizer),_imagePath(imagePath)
{
    rawImage = cv::imread(imagePath,CV_LOAD_IMAGE_COLOR);
    Init();
}

void SlicBaseDehaze::process()
{
    SlicSegmentation();
    GenerateDarkImage();

}

void SlicBaseDehaze::Init()
{
    _regionSize =((rawImage.cols>rawImage.rows)?rawImage.cols:rawImage.rows);
    _regionSize *=0.018;

    darkChannelImage = cv::Mat(rawImage.rows,rawImage.cols,CV_8UC1,cv::Scalar(0));
    transmission   = cv::Mat(rawImage.rows,rawImage.cols,CV_32FC1, cv::Scalar(0));

    cv::split(rawImage,channelLayers);
}

void SlicBaseDehaze::SlicSegmentation()
{
    clock_t t1 = clock();

    GDALAllRegister();
    CPLSetConfigOption("GDAL_FILENAME_IS_UTF8","NO");
    GDALDataset* poSrcDS = (GDALDataset*) GDALOpen(_imagePath.c_str(),GA_ReadOnly);
    if(poSrcDS == NULL)
    {
        std::cerr<<"Open File "<<_imagePath<<" Failed!"<<std::endl;
        return ;
    }
    GDALDriver* poDriver = (GDALDriver*)GDALGetDriverByName("GTiff");
    GDALDataset* poDstDS =  poDriver->Create(_segmentationPath.c_str(),poSrcDS->GetRasterXSize(),poSrcDS->GetRasterYSize(),1,GDT_Int32,NULL);
    if (poDstDS == NULL)
    {
        std::cerr<<"Create File "<<_segmentationPath<<" Failed!"<<std::endl;
        return ;
    }
    poDstDS->SetProjection(poSrcDS->GetProjectionRef());
    double adfGeoTransform[6];
    poSrcDS->GetGeoTransform(adfGeoTransform);
    poDstDS->SetGeoTransform(adfGeoTransform);

    Slic slic(poSrcDS,poDstDS,_regionSize,_regularizer);
    slic.StartSegmentation();

    GDALClose(poSrcDS);
    GDALClose(poDstDS);


    std::cout<<"cost "<<static_cast<double>(clock()-t1)/CLOCKS_PER_SEC <<"s"<<std::endl;
    return ;
}

cv::Mat &SlicBaseDehaze::GenerateDarkImage()
{
    cv::Mat segImage = cv::imread(_segmentationPath,CV_LOAD_IMAGE_ANYCOLOR|CV_LOAD_IMAGE_ANYDEPTH);
    std::cout <<segImage<<std::endl;
    return segImage;
}
