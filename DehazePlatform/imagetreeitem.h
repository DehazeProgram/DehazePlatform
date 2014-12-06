#ifndef IMAGETREEITEM_H
#define IMAGETREEITEM_H

#include <QStandardItem>

class ImageTreeItem : public QStandardItem
{
public:
    enum ItemType{
        IMAGE_ROOT,
        RED,
        GREEN,
        BLUE,
        DEHAZE_IMAGE_ROOT,
        DEHAZE_IMAGE
    };
    ImageTreeItem(ItemType itype,ImageTreeItem* parent=0,QString &text = QString(""));
    const ImageTreeItem* parent() const {return parentItem;}
    const ItemType getType() const{return _itemType;}

private:
    void initText(QString &text);
    void initFont();
    void initColor();

    ImageTreeItem* parentItem;
    ItemType _itemType;
    QString text;
};

#endif // IMAGETREEITEM_H
