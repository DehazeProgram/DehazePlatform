#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QPixmap>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <imagemanager.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    scene(new QGraphicsScene(this)),
    pxmapItem(NULL),
    imgManager(NULL)
{
    ui->setupUi(this);
    ui->mainToolBar->addAction(ui->actionOpen);
    ui->graphicsView->setScene(scene);
    ui->treeView->header()->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    if(imgManager)
    {
        imgManager =NULL;
    }
    QString imagePath = QFileDialog::getOpenFileName(this,"Selected a haze image");
    imgManager = new ImageManager(imagePath,this);
    ui->treeView->setModel((QAbstractItemModel*)(imgManager->getModel()) );
    imgManager->OpenImage();
    ui->treeView->expandAll();
}

void MainWindow::OpenImage(QString &imagePath)
{
    qDebug()<<imagePath;
    QPixmap p =  QPixmap(imagePath);
    if (pxmapItem)
        scene->removeItem(pxmapItem);
    pxmapItem = scene->addPixmap(p);
    ui->graphicsView->fitInView(pxmapItem,Qt::KeepAspectRatio);
    ui->graphicsView->ensureVisible(pxmapItem);
}




