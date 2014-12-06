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
    inline QString getImagePath(){ return imagePath;}

    void onContextMenu(QModelIndex &index);

public slots:
    void OpenImage();
    void ShowChannelImage(QModelIndex index);
    void RemoveImage();
    void LoadDehazeImage(MainWindow::DehazeType type);
signals:
    void OpenImage(QString&);
    void ColorImage(QString&);
    void GrayImage(QString&);
    void LoadDehazeImageItem(MainWindow::DehazeType,QString&);


private:
    QString imagePath;
    cv::Mat image;
    cv::Mat dehazeImage;
    MainWindow *window;
    ImageTreeModel *model;
    QGraphicsScene *scene;
    QAction* remove;

    void ShowImage(QPixmap &pixmap);
    void ConvertCvMatToGrayPixmaps(cv::Mat &mat, QVector<QPixmap> &pixmaps);
    QImage const copy_mat_to_qimage(cv::Mat const &mat);
};

#endif // IMAGEMANAGER_H
