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
    enum DehazeType{
        DARK_CHANNEL_DEHAZE
    };

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void DehazeFinish(MainWindow::DehazeType);

private slots:
    void on_actionOpen_triggered();
    void OpenImage(QString &imagePath);
    void on_treeView_customContextMenuRequested(const QPoint &pos);
    void on_DCDehazeButton_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsPixmapItem *pxmapItem;
    ImageManager *imgManager;

    void resizeEvent(QResizeEvent *event);
    void SetSpinBoxes();
    void SetDockWidgets();



};

#endif // MAINWINDOW_H
