#include "imagetreeitem.h"

ImageTreeItem::ImageTreeItem(QString &text, ImageTreeItem *p):
    parentItem(p)
{
    this->setText(text);
}
