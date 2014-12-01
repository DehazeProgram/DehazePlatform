#include "imagetreemodel.h"
#include "imagetreeitem.h"
#include <QDebug>

ImageTreeModel::ImageTreeModel(QObject *parent):
    QStandardItemModel(parent),
    rootItem(NULL)
{

}

ImageTreeModel::~ImageTreeModel()
{
    if (!rootItem)
        delete rootItem;
}


void ImageTreeModel::initImageTreeModel(QString &text)
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
