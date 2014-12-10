#ifndef DEHAZE_H
#define DEHAZE_H

#include <QVector>

#include <opencv2/opencv.hpp>

class Dehaze
{
public:
    Dehaze();
    virtual ~Dehaze() = 0;
    inline cv::Mat& GetDahezeImage() {return dehazeImage;}
private:
    cv::Mat dehazeImage;

};

#endif // DEHAZE_H
