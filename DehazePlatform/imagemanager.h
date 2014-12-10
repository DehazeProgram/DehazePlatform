#ifndef IMAGEMANAGER_H
#define IMAGEMANAGER_H
#include <QString>
#include <QObject>
#include <opencv2/opencv.hpp>
#include "dehaze.h"
#include "mainwindow.h"
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
    MainWindow *window;
    ImageTreeModel *model;
    QGraphicsScene *scene;
    QAction* remove;
    Dehaze* dehazeImages;

    void ShowImage(QPixmap &pixmap);
    void ShowImage();
    void ConvertCvMatToGrayPixmaps(const cv::Mat &mat, QVector<QPixmap> &pixmaps);
    void ConvertCvMatToGrayPixmaps(const cv::Mat &mat, QPixmap &pixmap);
    QImage const copy_mat_to_qimage(cv::Mat const &mat);
};

#endif // IMAGEMANAGER_H
