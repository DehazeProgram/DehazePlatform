#include "darkchanneldehazor.h"
#include "filter.h"
#include "colorcorrect.h"
#include "time.h"
#include <fstream>
#include <iostream>
#include <QVector>
#include <QString>
#include <QDebug>
#include <opencv2/opencv.hpp>


DarkChannelDehazor::DarkChannelDehazor(QString &imagePath,  float eps, float t, int max_a)
    :_eps(eps),_max_A(max_a),_t0(t)
{
    std::string path = imagePath.toStdString();
    rawImage =cv::imread(path,CV_LOAD_IMAGE_COLOR);
    Init();
}


DarkChannelDehazor::DarkChannelDehazor(const cv::Mat &img,float eps, float t, int max_a)
    :rawImage(img),_eps(eps),_max_A(max_a),_t0(t)
{
    CV_Assert(rawImage.channels() == 3);
    Init();
}

void DarkChannelDehazor::Init()
{
    _minFliterWindowSize =((rawImage.cols>rawImage.rows)?rawImage.cols:rawImage.rows);
    _minFliterWindowSize *=0.018;


    darkChannelImage = cv::Mat(rawImage.rows,rawImage.cols,CV_8UC1,cv::Scalar(0));
    transmission   = cv::Mat(rawImage.rows,rawImage.cols,CV_32FC1, cv::Scalar(0));

    cv::split(rawImage,channelLayers);
    

}

void DarkChannelDehazor::Process(cv::Mat &dehazeImage,cv::Mat &DCImage,cv::Mat &transmissionImage)
{
    dehazeImage = cv::Mat(rawImage.rows,rawImage.cols,CV_8UC1);
    DCImage = GenerateDarkImage();
    GenerateAtmosphericRadiation();
    transmissionImage = GenereteTransmmision().clone();
    GenerateDehazeImage(dehazeImage);
}

cv::Mat& DarkChannelDehazor::GenerateDarkImage()
{
    Filter::DarkImageFilter(rawImage,_minFliterWindowSize,darkChannelImage);
    return darkChannelImage;
}


//_max_A需要预先设定，这里设定的值为225
void DarkChannelDehazor::GenerateAtmosphericRadiation()
{
    int count = darkChannelImage.cols*darkChannelImage.rows;
    int edgeValue =255;
    QVector<int> darkHisto;
    ColorCorrect:: GenerateHistogram(darkHisto,darkChannelImage);
    while (darkHisto[edgeValue] >count*(1- 0.001)) {
        --edgeValue;
    }
    --edgeValue;

    int c=0;
    int tempB =0,tempG =0,tempR =0;
    for(int i =0;i< darkChannelImage.rows;++i)
    {
        for(int j =0;j< darkChannelImage.cols;++j)
        {
            if(darkChannelImage.at<uchar>(i,j) >= edgeValue)
            {
                tempB +=(int)(channelLayers[0].at<uchar>(i,j));
                tempG +=(int)(channelLayers[1].at<uchar>(i,j));
                tempR +=(int)(channelLayers[2].at<uchar>(i,j));
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

}

cv::Mat& DarkChannelDehazor::GenereteTransmmision()
{
    cv::Mat_<float> trans_t= cv::Mat(rawImage.rows,rawImage.cols,CV_32F, cv::Scalar(0));

    for(int i =0;i< transmission.rows;++i)
    {
        for(int j =0;j< transmission.cols;++j)
        {
            float dark = static_cast<float>(darkChannelImage.at<uchar>(i,j));
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
    Filter::GuideFilter_Single(channelLayers[0],trans_t,transmission,_minFliterWindowSize*4,_eps);
    return transmission;

}

void DarkChannelDehazor::GenerateDehazeImage(cv::Mat &dehazeImage)
{
    int row = rawImage.rows;
    int col = rawImage.cols;
    cv::Mat dehaze_b = cv::Mat(row,col,CV_8UC1);
    cv::Mat dehaze_g = cv::Mat(row,col,CV_8UC1);
    cv::Mat dehaze_r = cv::Mat(row,col,CV_8UC1);
    for(int i =0;i <row;++i)
    {
        for(int j=0;j< col;++j)
        {
            float t = transmission.at<float>(i,j);

            if(t <_t0)
                t= _t0;

            int temp_b = (int)((channelLayers[0].at<uchar>(i,j)-_A.b)/t +_A.b);
            int temp_g = (int)((channelLayers[1].at<uchar>(i,j)-_A.g)/t +_A.g);
            int temp_r = (int)((channelLayers[2].at<uchar>(i,j)-_A.r)/t +_A.r);

            temp_b = ((temp_b>_max_A)?channelLayers[0].at<uchar>(i,j):temp_b);
            temp_g = ((temp_g>_max_A)?channelLayers[1].at<uchar>(i,j):temp_g);
            temp_r = ((temp_r>_max_A)?channelLayers[2].at<uchar>(i,j):temp_r);

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

    cv::merge(dehazes,dehazeImage);
    cv::imwrite("C:\\hr\\experiment\\dehazeimage\\IMG_30154_dehaze6.jpg",dehazeImage);

}
