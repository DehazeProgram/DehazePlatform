#ifndef IMAGEMANAGER_H
#define IMAGEMANAGER_H
#include <QString>
#include <QObject>
#include <opencv2/opencv.hpp>
#include <mainwindow.h>
#include "ui_mainwindow.h"
//#include "imagetreemodel.h"


class ImageTreeModel;
class ImageManager:public QObject
{
    Q_OBJECT
    friend class MainWindow;
public:
    ImageManager(QString &imgPath,MainWindow* w);
    ~ImageManager();
    inline ImageTreeModel* getModel() {return model;}
    inline void setModel(ImageTreeModel* m) {model =m;}
    void onContextMenu(QModelIndex &index);

public slots:
    void OpenImage();
    void ShowChannelImage(QModelIndex index);
    void RemoveImage();
signals:
    void OpenImage(QString&);
    void ColorImage(QString&);
    void GrayImage(QString&);


private:
    QString imagePath;
    cv::Mat image;
    MainWindow *window;
    ImageTreeModel *model;
    QGraphicsScene *scene;
    QAction* remove;

};

#endif // IMAGEMANAGER_H
