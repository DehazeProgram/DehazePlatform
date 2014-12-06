#ifndef IMAGETREEMODEL_H
#define IMAGETREEMODEL_H

#include <QStandardItemModel>
#include "imagemanager.h"
#include "imagetreeitem.h"
#include <QVector>


class ImageTreeModel : public QStandardItemModel
{
public:
    Q_OBJECT
    friend class ImageManager;
    ImageTreeModel(QObject *parent =0);
    ~ImageTreeModel();

public slots:
    void InitImageTreeModel(QString &text);
    void LoadDehazeImage(MainWindow::DehazeType type, QString &text);
private:
    ImageTreeItem* rootItem;
    ImageTreeItem* DCDehazeImageItem;

};

#endif // IMAGETREEMODEL_H
