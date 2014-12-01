#include "imagemanager.h"
#include "imagetreemodel.h"
#include "imagetreeitem.h"
#include <QDebug>
#include <opencv2/opencv.hpp>
#include <QMenu>
#include <QAction>

ImageManager::ImageManager(QString &imgPath, MainWindow *w):
    QObject(NULL),
    imagePath(imgPath),
    window(w),
    scene(new QGraphicsScene(this)),
    model(new ImageTreeModel(NULL)),
    remove(new QAction(QString("Remove"),NULL))
{
    image = cv::imread(imagePath.toStdString());
    QObject::connect(this,SIGNAL(OpenImage(QString&)),window,SLOT(OpenImage(QString&)));
    QObject::connect(this,SIGNAL(ColorImage(QString&)),model,SLOT(initImageTreeModel(QString&)));
    QObject::connect(remove,SIGNAL(triggered()),this,SLOT(RemoveImage()));
}

ImageManager::~ImageManager()
{
    model->clear();
    scene->clear();
    image.release();
}

void ImageManager::onContextMenu(QModelIndex &index)
{
    /*when user right click on the tree view ,this function will be called
      the main function including remove,dehaze,rename and so on */

    ImageTreeItem* item = (ImageTreeItem*)(model->itemFromIndex(index));
    if(item->getType() == ImageTreeItem::IMAGE_ROOT)
    {
        QMenu* menu = new QMenu();
        menu->addAction(remove);
        menu->exec(QCursor::pos());
    }
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

void ImageManager::ShowChannelImage(QModelIndex index)
{
    /* when user doubleclick the item(like red,green,blue),
     * show the corresponding channel layer to users,
     * this slot function is connected with item's
     * doubleclicks signal in mainwindow.cpp */


    //split image to rbg as simple channel mats named red,green,blue
    cv::Mat red = cv::Mat(image.rows,image.cols,CV_8UC1);
    cv::Mat green = cv::Mat(image.rows,image.cols,CV_8UC1);
    cv::Mat blue = cv::Mat(image.rows,image.cols,CV_8UC1);
    std::vector<cv::Mat> channelImages;
    cv::split(image,channelImages);
    red = channelImages[0];
    green = channelImages[1];
    blue = channelImages[2];

    //create simple channel image as QImage set to scene
    ImageTreeItem* item = (ImageTreeItem*)(model->itemFromIndex(index));
    if(item->getType() == ImageTreeItem::RED)
    {
//        cv::imshow("red",red);
//        QImage image = QImage(red.data,red.rows,red.cols,QImage::Format_Indexed8);
        QPixmap pixmap;
        qDebug()<<pixmap.loadFromData(red.data,Qt::AutoColor);
        if (window->pxmapItem)
            scene->removeItem((QGraphicsItem*)(window->pxmapItem));
        window->pxmapItem = scene->addPixmap(pixmap);
        window->ui->graphicsView->fitInView((QGraphicsItem *)window->pxmapItem,Qt::KeepAspectRatio);
        window->ui->graphicsView->ensureVisible((QGraphicsItem *)window->pxmapItem);
    }
    else if (item->getType() == ImageTreeItem::BLUE)
    {

    }
    else if (item->getType() == ImageTreeItem::GREEN)
    {

    }
}

void ImageManager::RemoveImage()
{
    model->clear();
    scene->clear();
    image.release();
}

