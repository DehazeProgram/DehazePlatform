#ifndef IMAGETREEITEM_H
#define IMAGETREEITEM_H

#include <QStandardItem>

class ImageTreeItem : public QStandardItem
{
public:
    ImageTreeItem(QString &text,ImageTreeItem*p=0);
    ImageTreeItem* parent() const {return parentItem;}

private:
    ImageTreeItem* parentItem;
};

#endif // IMAGETREEITEM_H
