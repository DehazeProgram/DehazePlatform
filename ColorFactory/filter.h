#ifndef FILTER_H
#define FILTER_H

#include <opencv2/opencv.hpp>
#include <QVector>
#include <QPoint>
#include <deque>
#include"time.h"
#include "colorfactory_global.h"



class FILTER_SHARED_EXPORT Filter
{
public:
    Filter();
    static void DarkImageFilter(cv::Mat &src, int windowsize, cv::Mat &dst);
    static void GuideFilter_Single(cv::Mat& guidedImage, cv::Mat& source,cv::Mat& output,int radius, float epsilon);
    static void GuideFilter_Multi(QVector<cv::Mat> &guidedImages, cv::Mat& sourse, cv::Mat& output, int radius, float epsilon);
    static void MaxFilter_2D(cv::Mat &src,cv::Mat &dst,int windowsize);
    static void MinFilter_2D(cv::Mat &src,cv::Mat &dst,int windowsize);
private:
    template<typename T>
    static int max(const T& a,const T& b){
        if(a>b)
            return a;
        return b;
    }

    template<typename T>
    static int min(const T& a,const T& b){
        if(a>b)
            return b;
        return a;
    }
    template<typename T>
    static void MinFilter_1D(std::vector<T> &vec, std::vector<T>&dst,int windowsize,std::deque<int> &mins=std::deque<int>());
    template<typename T>
    static void MaxFilter_1D(std::vector<T> &vec, std::vector<T> &dst, int windowsize, std::deque<int> &maxs=std::deque<int>());
    static void MakeDepth32f(cv::Mat& source, cv::Mat& output);

};

#endif // FILTER_H
