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
    pxmapItem(NULL),
    imgManager(NULL)
{
    ui->setupUi(this);
    ui->mainToolBar->addAction(ui->actionOpen);
    ui->treeView->header()->hide();
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    if(imgManager)
    {
        delete imgManager;
    }
    QString imagePath = QFileDialog::getOpenFileName(this,"Selected a haze image");
    imgManager = new ImageManager(imagePath,this);
    ui->graphicsView->setScene(imgManager->scene);
    ui->treeView->setModel((QAbstractItemModel*)(imgManager->getModel()) );
    imgManager->OpenImage();
    ui->treeView->expandAll();
    connect(ui->treeView,SIGNAL(doubleClicked(QModelIndex)),imgManager,SLOT(ShowChannelImage(QModelIndex)));

}

void MainWindow::OpenImage(QString &imagePath)
{
    QPixmap p =  QPixmap(imagePath);
    pxmapItem = imgManager->scene->addPixmap(p);
    ui->graphicsView->fitInView(pxmapItem,Qt::KeepAspectRatio);
    ui->graphicsView->ensureVisible(pxmapItem);
}


void MainWindow::on_treeView_customContextMenuRequested(const QPoint &pos)
{
    QModelIndex index = ui->treeView->indexAt(pos);
    imgManager->onContextMenu(index);
}
