#ifndef IMAGETREEMODEL_H
#define IMAGETREEMODEL_H

#include <QStandardItemModel>
#include <imagemanager.h>

class ImageTreeItem;

class ImageTreeModel : public QStandardItemModel
{
public:
    Q_OBJECT
    friend class ImageManager;
    ImageTreeModel(QObject *parent =0);
    ~ImageTreeModel();

public slots:
    void initImageTreeModel(QString &text);
private:
    ImageTreeItem* rootItem;
};

#endif // IMAGETREEMODEL_H
