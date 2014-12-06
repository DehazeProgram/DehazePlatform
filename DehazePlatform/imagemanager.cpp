#include "imagemanager.h"
#include "imagetreemodel.h"
#include "imagetreeitem.h"
#include <QDebug>
#include <opencv2/opencv.hpp>
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
    remove(new QAction(QString("Remove"),NULL))
{
    image = cv::imread(imagePath.toStdString());
    QObject::connect(this,SIGNAL(OpenImage(QString&)),window,SLOT(OpenImage(QString&)));
    QObject::connect(this,SIGNAL(ColorImage(QString&)),model,SLOT(InitImageTreeModel(QString&)));
    QObject::connect(remove,SIGNAL(triggered()),this,SLOT(RemoveImage()));
    QObject::connect(this,SIGNAL(LoadDehazeImageItem(MainWindow::DehazeType,QString&)),model,SLOT(LoadDehazeImage(MainWindow::DehazeType,QString&)));
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

    //create simple channel image as QImage set to scene
    QVector<QPixmap> channelPixmaps;
    QPixmap redPixmap = QPixmap();
    QPixmap greenPixmap = QPixmap();
    QPixmap bluePixmap = QPixmap();
    channelPixmaps.push_back(redPixmap);
    channelPixmaps.push_back(greenPixmap);
    channelPixmaps.push_back(bluePixmap);

    ConvertCvMatToGrayPixmaps(image,channelPixmaps);
    ImageTreeItem* item = (ImageTreeItem*)(model->itemFromIndex(index));
    if(item->getType() == ImageTreeItem::IMAGE_ROOT)
    {
        QPixmap pixmap =QPixmap(imagePath);
        ShowImage(pixmap);
    }
    else if(item->getType() == ImageTreeItem::RED)
    {
        ShowImage(channelPixmaps[0]);
    }
    else if (item->getType() == ImageTreeItem::BLUE)
    {
        ShowImage(channelPixmaps[1]);
    }
    else if (item->getType() == ImageTreeItem::GREEN)
    {
        ShowImage(channelPixmaps[2]);
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

void ImageManager::ConvertCvMatToGrayPixmaps(cv::Mat &mat,QVector<QPixmap> &pixmaps)
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

const QImage ImageManager::copy_mat_to_qimage(const cv::Mat &mat)
{
    QImage image(mat.cols, mat.rows, QImage::Format_RGB888);
    for (int i = 0; i != mat.rows; ++i)
    {
        memcpy(image.scanLine(i), mat.ptr(i), image.bytesPerLine() );
    }
    return image;
}

