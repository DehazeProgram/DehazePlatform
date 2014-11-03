#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QPixmap>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    scene(new QGraphicsScene(this)),
    pxmapItem(NULL),
    toolbar(new QToolBar("tools"))
{
    this->addToolBar(Qt::LeftToolBarArea,toolbar);
    ui->setupUi(this);
    ui->mainToolBar->addAction(ui->actionOpen);
    ui->graphicsView->setScene(scene);

    toolbar->addAction("Dehaze");
    toolbar->addAction("Autocolor");
    toolbar->addAction("Autocontract");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    QString imagePath = QFileDialog::getOpenFileName(this,"Selected a haze image");
    qDebug()<<imagePath;
    QPixmap p =  QPixmap(imagePath);
    if (pxmapItem)
        scene->removeItem(pxmapItem);
    pxmapItem = scene->addPixmap(p);
}
