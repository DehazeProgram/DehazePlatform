#include "slicbasedehaze.h"
#include "opencv2/opencv.hpp"
#include <map>
#include <vector>
#include <iterator>
#include <utility>
#include "colorcorrect.h"
#include "filter.h"

SlicBaseDehaze::SlicBaseDehaze(std::string &imagePath, std::string &segPath,  double m,float eps, float t, uchar max_a):
    _imagePath(imagePath),_segPath(segPath),_m(m),_eps(eps),_t0(t),_max_A(max_a)
{
    Init();
}


void SlicBaseDehaze::Init()
{
    _rawImage = cv::imread(_imagePath,CV_LOAD_IMAGE_COLOR);
    _darkChannelImage = cv::Mat(_rawImage.rows,_rawImage.cols,CV_8UC1,cv::Scalar(0));
    _dehazeImage = cv::Mat(_rawImage.rows,_rawImage.cols,CV_8UC1,cv::Scalar(0));
    _transmission   = cv::Mat(_rawImage.rows,_rawImage.cols,CV_32FC1, cv::Scalar(0));
    _r =((_rawImage.cols>_rawImage.rows)?_rawImage.cols:_rawImage.rows);
    _r *=0.018;
    _r = (_r/10+1)*10;

    cv::split(_rawImage,_channelLayers);
}

void SlicBaseDehaze::Process()
{
    clock_t t1 = clock();
    Segmentation(_imagePath.c_str(),_segPath.c_str(),_r,_m);
    GenerateDarkImage();
    GenerateAtmosphericRadiation();
    GenereteTransmmision();
    GenerateDehazeImage();
    std::cout<<"cost "<<static_cast<double>(clock()-t1)/CLOCKS_PER_SEC <<"s"<<std::endl;

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

void SlicBaseDehaze::GenerateDarkImage()
{
    cv::Mat sgmMat = cv::imread(_segPath,CV_LOAD_IMAGE_ANYCOLOR|CV_LOAD_IMAGE_ANYDEPTH);

    std::map<int,std::vector<uchar> > mark_pixels;
    std::map<int,uchar> mark_min;
    for(int i = 0; i< sgmMat.rows;++i)
    {
        for(int j = 0; j< sgmMat.cols;++j)
        {
            cv::Vec3b vec = _rawImage.at<cv::Vec3b>(i,j);
            int key = sgmMat.at<int>(i,j);
            if(mark_pixels.count(key) == 0)
            {
                mark_pixels[key] = std::vector<uchar>();
            }
            mark_pixels[key].push_back(vec.val[0]);
            mark_pixels[key].push_back(vec.val[1]);
            mark_pixels[key].push_back(vec.val[2]);

        }
    }
    for(std::map<int,std::vector<uchar> >::iterator iter = mark_pixels.begin();iter != mark_pixels.end();++iter)
    {
        std::vector<uchar> pixelsValue = iter->second;
        uchar min = pixelsValue[0];
        for(int i =0;i <pixelsValue.size();++i)
        {
            if(pixelsValue[i] <min)
                min = pixelsValue[i];
        }
        mark_min[iter->first] = min;
    }
    for(int i = 0; i< _darkChannelImage.rows;++i)
    {
        for(int j = 0; j< _darkChannelImage.cols;++j)
        {
            _darkChannelImage.at<uchar>(i,j) = mark_min[sgmMat.at<int>(i,j)];
        }
    }

}

void SlicBaseDehaze::GenerateAtmosphericRadiation()
{
    int count = _darkChannelImage.cols*_darkChannelImage.rows;
    int edgeValue =255;
    QVector<int> darkHisto;
    ColorCorrect:: GenerateHistogram(darkHisto,_darkChannelImage);
    while (darkHisto[edgeValue] >count*(1- 0.001)) {
        --edgeValue;
    }
    --edgeValue;

    int c=0;
    int tempB =0,tempG =0,tempR =0;
    for(int i =0;i< _darkChannelImage.rows;++i)
    {
        for(int j =0;j< _darkChannelImage.cols;++j)
        {
            if(_darkChannelImage.at<uchar>(i,j) >= edgeValue)
            {
                tempB +=(int)(_channelLayers[0].at<uchar>(i,j));
                tempG +=(int)(_channelLayers[1].at<uchar>(i,j));
                tempR +=(int)(_channelLayers[2].at<uchar>(i,j));
                ++c;
            }
        }
    }
    tempB /=c;
    tempG /=c;
    tempR /=c;

    _A.b = ((tempB > _max_A)?_max_A:tempB);
    _A.g = ((tempG > _max_A)?_max_A:tempG);
    _A.r = ((tempR > _max_A)?_max_A:tempR);
    std::cout <<_A.b<<" "<<_A.g<<" "<<_A.r<<" "<<std::endl;
}

void SlicBaseDehaze::GenereteTransmmision()
{
    cv::Mat_<float> trans_t= cv::Mat(_rawImage.rows,_rawImage.cols,CV_32F, cv::Scalar(0));

    for(int i =0;i< _transmission.rows;++i)
    {
        for(int j =0;j< _transmission.cols;++j)
        {
            float dark = static_cast<float>(_darkChannelImage.at<uchar>(i,j));
            float temp_b =static_cast<float>(1.0 - _eps*(dark/_A.b));
            float temp_g =static_cast<float>(1.0 - _eps*(dark/_A.g));
            float temp_r =static_cast<float>(1.0 - _eps*(dark/_A.r));
            float t;

            if(temp_b <0) temp_b =0;
            else if(temp_b >1) temp_b =1;

            if(temp_g <0) temp_g =0;
            else if(temp_g >1) temp_g =1;

            if(temp_r <0) temp_r =0;
            else if(temp_r >1) temp_r =1;

            t =((temp_b>temp_g)?temp_b:temp_g);
            t =((t >temp_r)?t:temp_r);
            trans_t.at<float>(i,j) =t;
        }
    }
    Filter::GuideFilter_Single(_channelLayers[1],trans_t,_transmission,_r*4,_eps);
}

void SlicBaseDehaze::GenerateDehazeImage()
{
    int row = _rawImage.rows;
    int col = _rawImage.cols;
    cv::Mat dehaze_b = cv::Mat(row,col,CV_8UC1);
    cv::Mat dehaze_g = cv::Mat(row,col,CV_8UC1);
    cv::Mat dehaze_r = cv::Mat(row,col,CV_8UC1);
    for(int i =0;i <row;++i)
    {
        for(int j=0;j< col;++j)
        {
            float t = _transmission.at<float>(i,j);

            if(t <_t0)
                t= _t0;

            int temp_b = (int)((_channelLayers[0].at<uchar>(i,j)-_A.b)/t +_A.b);
            int temp_g = (int)((_channelLayers[1].at<uchar>(i,j)-_A.g)/t +_A.g);
            int temp_r = (int)((_channelLayers[2].at<uchar>(i,j)-_A.r)/t +_A.r);

            temp_b = ((temp_b>_max_A)?_channelLayers[0].at<uchar>(i,j):temp_b);
            temp_g = ((temp_g>_max_A)?_channelLayers[1].at<uchar>(i,j):temp_g);
            temp_r = ((temp_r>_max_A)?_channelLayers[2].at<uchar>(i,j):temp_r);

            temp_b = ((temp_b <0)?0:temp_b);
            temp_g = ((temp_g <0)?0:temp_g);
            temp_r = ((temp_r <0)?0:temp_r);

            dehaze_b.at<uchar>(i,j) = temp_b;
            dehaze_g.at<uchar>(i,j) = temp_g;
            dehaze_r.at<uchar>(i,j) = temp_r;
        }
    }

    std::vector<cv::Mat> dehazes;
    dehazes.push_back(dehaze_b);
    dehazes.push_back(dehaze_g);
    dehazes.push_back(dehaze_r);

    cv::merge(dehazes,_dehazeImage);
    cv::imwrite("C:\\hr\\experiment\\dehazeimage\\IMG_30154_dehaze12.jpg",_dehazeImage);
}


