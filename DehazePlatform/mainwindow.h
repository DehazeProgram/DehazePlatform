#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


namespace Ui {
class MainWindow;
}
class QGraphicsScene;
class QGraphicsPixmapItem;
class ImageManager;
class MainWindow : public QMainWindow
{
    Q_OBJECT
    friend class ImageManager;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();
    void OpenImage(QString &imagePath);
    void on_treeView_customContextMenuRequested(const QPoint &pos);

private:
    Ui::MainWindow *ui;
    QGraphicsPixmapItem *pxmapItem;
    ImageManager *imgManager;



};

#endif // MAINWINDOW_H
