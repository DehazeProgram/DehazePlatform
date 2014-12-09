#include "imagetreemodel.h"
#include "imagetreeitem.h"
#include <QDebug>

ImageTreeModel::ImageTreeModel(QObject *parent):
    QStandardItemModel(parent),
    rootItem(NULL),
    DCDehazeImageItem(NULL)
{

}

ImageTreeModel::~ImageTreeModel()
{
    if (!rootItem)
        delete rootItem;
}


void ImageTreeModel::InitImageTreeModel(QString &text)
{
    if(!rootItem)
        rootItem =NULL;
    rootItem = new ImageTreeItem(ImageTreeItem::IMAGE_ROOT,NULL,text);

    ImageTreeItem* redItem = new ImageTreeItem(ImageTreeItem::RED,rootItem);
    ImageTreeItem* greenItem = new ImageTreeItem(ImageTreeItem::GREEN,rootItem);
    ImageTreeItem* blueItem = new ImageTreeItem(ImageTreeItem::BLUE,rootItem);

    rootItem->appendRow(redItem);
    rootItem->appendRow(greenItem);
    rootItem->appendRow(blueItem);

    rootItem -> setEditable(false);
    redItem  -> setEditable(false);
    greenItem-> setEditable(false);
    blueItem -> setEditable(false);

    QPixmap red(QSize(12,12));
    red.fill(QColor(255,0,0));
    redItem->setIcon(QIcon(red));

    QPixmap blue(QSize(12,12));
    blue.fill(QColor(0,0,255));
    blueItem->setIcon(QIcon(blue));

    QPixmap green(QSize(12,12));
    green.fill(QColor(0,255,0));
    greenItem->setIcon(QIcon(green));

    this->appendRow(rootItem);
}

void ImageTreeModel::LoadDehazeImage(MainWindow::DehazeType type,QString& text)
{
    switch (type) {
    case MainWindow::DARK_CHANNEL_DEHAZE:
        if(!DCDehazeImageItem)
        {
            DCDehazeImageItem = new ImageTreeItem
                    (ImageTreeItem::DEHAZE_IMAGE_ROOT,rootItem,QString("Dark Channel Dehaze Image"));
            rootItem->appendRow(DCDehazeImageItem);
        }
        ImageTreeItem* imageItem = new ImageTreeItem(ImageTreeItem::DEHAZE_IMAGE,DCDehazeImageItem,text);
        ImageTreeItem* darkChannelItem = new ImageTreeItem(ImageTreeItem::DEHAZE_IMAGE_DCIMAGE,imageItem,QString("DarkChannelImage"));
        ImageTreeItem* refineImage = new ImageTreeItem(ImageTreeItem::DEHAZE_IMAGE_TRANSMISSION_IMAGE,imageItem,QString("TransmissionImage"));
        imageItem->appendRow(darkChannelItem);
        imageItem->appendRow(refineImage);
        DCDehazeImageItem->appendRow(imageItem);
        imageItem->setEditable(false);
        break;
    }

}


