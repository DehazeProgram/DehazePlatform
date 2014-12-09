#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QPixmap>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include "imagemanager.h"
#include "darkchanneldehazor.h"
#include "dehaze.h"
#include "dcdehaze.h"

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

    SetDockWidgets();
    SetSpinBoxes();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    QString imagePath = QFileDialog::getOpenFileName(this,"Selected a haze image");
    if(imagePath.isEmpty())
        return;
    if(imgManager)
    {
        delete imgManager;
    }
    imgManager = new ImageManager(imagePath,this);
    ui->graphicsView->setScene(imgManager->scene);
    ui->treeView->setModel((QAbstractItemModel*)(imgManager->getModel()) );
    imgManager->OpenImage();
    ui->treeView->expandAll();

    connect(ui->treeView,SIGNAL(doubleClicked(QModelIndex)),imgManager,SLOT(ShowChannelImage(QModelIndex)));
    connect(this,SIGNAL(DehazeFinish(MainWindow::DehazeType)),imgManager,SLOT(LoadDehazeImage(MainWindow::DehazeType)));
}


void MainWindow::on_treeView_customContextMenuRequested(const QPoint &pos)
{
    QModelIndex index = ui->treeView->indexAt(pos);
    imgManager->onContextMenu(index);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    /*make the image change the size when mainwindow change the size*/
     QWidget::resizeEvent(event);
     if(pxmapItem)
     {
         ui->graphicsView->fitInView(pxmapItem,Qt::KeepAspectRatio);
         ui->graphicsView->ensureVisible(pxmapItem);
         ui->graphicsView->show();
     }
}

void MainWindow::SetSpinBoxes()
{
    ui->DCEpsSpinBox->setMaximum(1.00);
    ui->DCEpsSpinBox->setMinimum(0.00);
    ui->DCEpsSpinBox->setSingleStep(0.01);
    ui->DCEpsSpinBox->setValue(1.00);
    ui->DCEpsSpinBox->setWrapping(true);

    ui->DCMaxASpinBox->setMaximum(255);
    ui->DCMaxASpinBox->setMinimum(0);
    ui->DCMaxASpinBox->setSingleStep(10);
    ui->DCMaxASpinBox->setValue(220);
    ui->DCMaxASpinBox->setWrapping(true);

    ui->DCTransmissionSpinBox->setMaximum(1.00);
    ui->DCTransmissionSpinBox->setMinimum(0);
    ui->DCTransmissionSpinBox->setSingleStep(0.01);
    ui->DCTransmissionSpinBox->setValue(0.01);
    ui->DCTransmissionSpinBox->setWrapping(true);
}

void MainWindow::SetDockWidgets()
{
    //setting dockWidget
    ui->dockWidget->setWindowTitle(QString("Image"));
    ui->darkChannelDock->setWindowTitle(QString("DarkChannelDehazor"));
    ui->colorCorrectDock->setWindowTitle(QString("ColorCorrect"));
//    this->tabifyDockWidget(ui->colorCorrectDock,ui->darkChannelDock);
//    ui->darkChannelDock->raise();
}

void MainWindow::on_DCDehazeButton_clicked()
{
    if(imgManager == NULL)
        return;
    imgManager->dehazeImages = new DCDehaze();
    DarkChannelDehazor dehazor(imgManager->getImagePath(),
                               ui->DCEpsSpinBox->value(),
                               ui->DCTransmissionSpinBox->value(),
                               ui->DCMaxASpinBox->value());
    dehazor.Process(imgManager->dehazeImages->GetDahezeImage(),
                    ((DCDehaze*)(imgManager->dehazeImages))->GetDarkChannelImage(),
                    ((DCDehaze*)(imgManager->dehazeImages))->GetTransmissionImage());
    emit DehazeFinish(DARK_CHANNEL_DEHAZE);
    ui->treeView->expandAll();
}
