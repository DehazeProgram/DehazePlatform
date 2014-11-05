#include "imagemanager.h"
#include "imagetreemodel.h"

ImageManager::ImageManager(QString &imgPath, MainWindow *w):
    QObject(NULL),
    imagePath(imgPath),
    window(w),
    model(new ImageTreeModel())
{
    image = cv::imread(imagePath.toStdString());
    QObject::connect(this,SIGNAL(OpenImage(QString&)),window,SLOT(OpenImage(QString&)));
    QObject::connect(this,SIGNAL(ColorImage(QString&)),model,SLOT(initImageTreeModel(QString&)));
}

void ImageManager::OpenImage()
{
    emit OpenImage(QString(imagePath));

    std::string imageName(imagePath.toStdString(),imagePath.toStdString().rfind("/")+1,
                          imagePath.size() -imagePath.toStdString().rfind("/"));
    if(image.channels() == 3)
        emit ColorImage(QString(imageName.c_str()));
    if(image.channels() == 1)
        emit GrayImage(QString(imageName.c_str()));
}

