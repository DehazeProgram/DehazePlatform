#ifndef COLORCORRECT_H
#define COLORCORRECT_H

#include<iostream>
#include<QVector>
#include<opencv2/opencv.hpp>
#include "colorfactory_global.h"


class COLORCORRECT_SHARED_EXPORT ColorCorrect
{
public:
    ColorCorrect();
    static void AutoColor(cv::Mat &image, float s1, float s2);
    static void AutoContract(cv::Mat &image, float s1,float s2);
    static void ContractEnhancement(cv::Mat &image, float s1, float s2, float eps);
    static void GenerateHistogram(QVector<int> &histo, const cv::Mat &image);
private:
    static void AutoColor_single(cv::Mat &image, float s1, float s2);

};

#endif // COLORCORRECT_H
