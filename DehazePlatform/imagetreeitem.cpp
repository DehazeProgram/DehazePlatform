#include "imagetreeitem.h"

ImageTreeItem::ImageTreeItem(ItemType itype, ImageTreeItem *parent, QString &text):
    parentItem(parent)
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
    case DC_DEHAZE_IMAGE_ROOT:
        this->setText(text);
        break;
    case DEHAZE_IMAGE:
        this->setText(text);
        break;
    case DEHAZE_IMAGE_DCIMAGE:
        this->setText(text);
        break;
    case DEHAZE_IMAGE_TRANSMISSION_IMAGE:
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
    case DC_DEHAZE_IMAGE_ROOT:
        font.setBold(true);
        font.setPointSize(font.pointSize()+1);
        break;
    case DEHAZE_IMAGE:
        font.setBold(true);
        font.setPointSize(font.pointSize());
        break;
    case DEHAZE_IMAGE_TRANSMISSION_IMAGE:
        font.setBold(true);
        font.setPointSize(font.pointSize());
        break;
    case DEHAZE_IMAGE_DCIMAGE:
        font.setBold(true);
        font.setPointSize(font.pointSize());
        break;
    case RED:
        font.setBold(true);
        font.setPointSize(font.pointSize());
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
    case DC_DEHAZE_IMAGE_ROOT:
        brush.setColor(QColor(Qt::red));
        break;
    case DEHAZE_IMAGE:
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
