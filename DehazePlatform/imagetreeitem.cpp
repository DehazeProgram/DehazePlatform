#include "imagetreeitem.h"

ImageTreeItem::ImageTreeItem(ItemType itype, ImageTreeItem *p, QString &text):
    parentItem(p)
{
    _itemType = itype;
    initText(text);
    initFont();
    initColor();
}

void ImageTreeItem::initText(QString &text)
{
    switch (_itemType) {
    case IMAGE_ROOT:
        this->setText(text);
        break;
    case RED:
        this->setText("red");
        break;
    case GREEN:
        this->setText("green");
        break;
    case BLUE:
        this->setText("blue");
        break;
    default:
        break;
    }
}

void ImageTreeItem::initFont()
{
    QFont font= this->font();
    switch (_itemType) {
    case IMAGE_ROOT:
        font.setBold(true);
        font.setPointSize(font.pointSize()+2);
        break;
    case RED:
        font.setBold(true);
        font.setPointSize(font.pointSize()+1);
        break;
    case GREEN:
        font.setBold(true);
        font.setPointSize(font.pointSize());
        break;
    case BLUE:
        font.setBold(true);
        break;
    default:
        break;
    }
    this->setFont(font);
}

void ImageTreeItem::initColor()
{

    QBrush brush = this->foreground();
    switch (_itemType) {
    case IMAGE_ROOT:
        brush.setColor(QColor(Qt::red));
        break;
    case RED:
        break;
    case GREEN:
        break;
    case BLUE:
        break;
    default:
        break;
    }
    this->setForeground(brush);

}
