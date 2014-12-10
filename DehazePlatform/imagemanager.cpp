#include "imagemanager.h"
#include "imagetreemodel.h"
#include "imagetreeitem.h"
#include "dcdehaze.h"
#include <QDebug>
#include <opencv2/opencv.hpp>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QMenu>
#include <QAction>
#include <QVector>
#include <QRgb>

ImageManager::ImageManager(QString &imgPath, MainWindow *w):
    QObject(NULL),
    imagePath(imgPath),
    window(w),
    scene(new QGraphicsScene(this)),
    model(new ImageTreeModel(NULL)),
    remove(new QAction(QString("Remove"),NULL)),
    dehazeImages(NULL)
{
    image = cv::imread(imagePath.toStdString());
    QObject::connect(this,SIGNAL(ColorImage(QString&)),model,SLOT(InitImageTreeModel(QString&)));
    QObject::connect(remove,SIGNAL(triggered()),this,SLOT(RemoveImage()));
    QObject::connect(this,SIGNAL(LoadDehazeImageItem(MainWindow::DehazeType,QString&)),model,SLOT(LoadDehazeImage(MainWindow::DehazeType,QString&)));
}

ImageManager::~ImageManager()
{
    delete dehazeImages;
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
    ShowImage();
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

    ImageTreeItem* item = (ImageTreeItem*)(model->itemFromIndex(index));

    //create simple channel image as QImage set to scene
    if ((item->getType() == ImageTreeItem::IMAGE_ROOT||item->parent()->getType() == ImageTreeItem::IMAGE_ROOT))
    {
        QVector<QPixmap> channelPixmaps;
        QPixmap redPixmap = QPixmap();
        QPixmap greenPixmap = QPixmap();
        QPixmap bluePixmap = QPixmap();
        channelPixmaps.push_back(redPixmap);
        channelPixmaps.push_back(greenPixmap);
        channelPixmaps.push_back(bluePixmap);

        ConvertCvMatToGrayPixmaps(image,channelPixmaps);
        switch (item->getType()) {
        case ImageTreeItem::IMAGE_ROOT:
        {
            ShowImage();
            break;
        }
        case ImageTreeItem::RED:
            ShowImage(channelPixmaps[0]);
            break;
        case ImageTreeItem::BLUE:
            ShowImage(channelPixmaps[1]);
            break;
        case ImageTreeItem::GREEN:
            ShowImage(channelPixmaps[2]);
            break;
        }
    }
    else if (item->parent()->getType() == ImageTreeItem::DC_DEHAZE_IMAGE_ROOT||item->parent()->parent()->getType() == ImageTreeItem::DC_DEHAZE_IMAGE_ROOT)
    {
        switch (item->getType()) {
        case ImageTreeItem::DEHAZE_IMAGE:
        {
            QPixmap pixmap;
            ConvertCvMatToGrayPixmaps(dehazeImages->GetDahezeImage(),pixmap);
            ShowImage(pixmap);
            break;
        }

        case ImageTreeItem::DEHAZE_IMAGE_DCIMAGE:
        {
            QPixmap pixmap;
            ConvertCvMatToGrayPixmaps(((DCDehaze*)dehazeImages)->GetDarkChannelImage(),pixmap);
            ShowImage(pixmap);
            break;
        }

        case ImageTreeItem::DEHAZE_IMAGE_TRANSMISSION_IMAGE:
        {
            QPixmap pixmap;
            cv::Mat mat = ((DCDehaze*)dehazeImages)->GetTransmissionImage();
            cv::Mat mat_255 = cv::Mat(mat.rows,mat.cols,CV_32FC1,cv::Scalar(255.0));
            cv::Mat mat_UC8 = cv::Mat(mat.rows,mat.cols,CV_32FC1) ;
            cv::multiply(mat,mat_255,mat_UC8);
            mat_UC8.convertTo(mat_UC8,CV_8UC1);
            ConvertCvMatToGrayPixmaps(mat_UC8,pixmap);
            ShowImage(pixmap);
            break;
        }
        }
    }
    else
    {
        qDebug()<<"haha";
    }

}

void ImageManager::RemoveImage()
{
    model->clear();
    scene->clear();
    image.release();
}

void ImageManager::LoadDehazeImage(MainWindow::DehazeType type)
{

    emit LoadDehazeImageItem(type,window->ui->DCNamelineEdit->text());
}

void ImageManager::ShowImage(QPixmap &pixmap)
{
    if (window->pxmapItem)
        scene->removeItem((QGraphicsItem*)(window->pxmapItem));

    window->pxmapItem = scene->addPixmap(pixmap);
    window->ui->graphicsView->setScene(scene);
    window->ui->graphicsView->fitInView((QGraphicsItem *)window->pxmapItem,Qt::KeepAspectRatio);
    window->ui->graphicsView->ensureVisible((QGraphicsItem *)window->pxmapItem);

}

void ImageManager::ShowImage()
{

    if (window->pxmapItem)
        scene->removeItem((QGraphicsItem*)(window->pxmapItem));
    QPixmap p =  QPixmap(imagePath);
    p.load(imagePath);
    if(p.isNull())
        qDebug()<<"p is null";
    window->pxmapItem = window->imgManager->scene->addPixmap(p);
    window->ui->graphicsView->fitInView(window->pxmapItem,Qt::KeepAspectRatio);
    window->ui->graphicsView->ensureVisible(window->pxmapItem);
    window->ui->graphicsView->show();
}

void ImageManager::ConvertCvMatToGrayPixmaps(const cv::Mat &mat,QVector<QPixmap> &pixmaps)
{
    assert(mat.channels() == 3 && mat.type() == CV_8UC3);
    QImage img = copy_mat_to_qimage(mat);
    QSize imgSize = img.size();
    int width = img.width();
    int height = img.height();
    QImage rImg(imgSize, QImage::Format_RGB888);
    QImage gImg(imgSize, QImage::Format_RGB888);
    QImage bImg(imgSize, QImage::Format_RGB888);

    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            QRgb rgb = img.pixel(i,j);
            rImg.setPixel(i,j,qRgb(qBlue(rgb),qBlue(rgb),qBlue(rgb)));
            gImg.setPixel(i,j,qRgb(qGreen(rgb),qGreen(rgb),qGreen(rgb)));
            bImg.setPixel(i,j,qRgb(qRed(rgb),qRed(rgb),qRed(rgb)));
        }
    }
    pixmaps[0] = QPixmap::fromImage(rImg);
    pixmaps[1] = QPixmap::fromImage(gImg);
    pixmaps[2] = QPixmap::fromImage(bImg);

}

void ImageManager::ConvertCvMatToGrayPixmaps(const cv::Mat &mat, QPixmap &pixmap)
{
    assert((mat.channels() == 3 || mat.channels() == 1)&& mat.type() == CV_8UC3);
    if(mat.channels() == 3)
    {
        std::vector<cv::Mat> vec;
        cv::split(mat,vec);
        QSize size(mat.cols,mat.rows);
        QImage img(size,QImage::Format_RGB888);

        for(int i = 0;i < mat.rows; ++i)
        {
            for(int j = 0;j < mat.cols; ++j)
            {
                int r = vec[2].at<uchar>(i,j);
                int g = vec[1].at<uchar>(i,j);
                int b = vec[0].at<uchar>(i,j);
                img.setPixel(j,i,qRgb(r,g,b));
            }
        }
        pixmap = QPixmap::fromImage(img);
        return;
    }
    else if(mat.channels() ==1)
    {
        QSize size(mat.cols,mat.rows);
        QImage img(size,QImage::Format_RGB888);

        for(int i = 0;i < mat.rows; ++i)
        {
            for(int j = 0;j < mat.cols; ++j)
            {
                int gray = mat.at<uchar>(i,j);
                img.setPixel(j,i,qRgb(gray,gray,gray));
            }
        }
        pixmap = QPixmap::fromImage(img);
        return;
    }
}


const QImage ImageManager::copy_mat_to_qimage(const cv::Mat &mat)
{
    QImage image(mat.cols, mat.rows, QImage::Format_RGB888);
    for (int i = 0; i != mat.rows; ++i)
    {
        memcpy(image.scanLine(i), mat.ptr(i), image.bytesPerLine() );
    }
    return image;
}

