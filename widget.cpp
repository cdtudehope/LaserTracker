#include "widget.h"
#include "ui_widget.h"
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QVBoxLayout>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QCameraInfo>
#include <string>
#include <iostream>
//#include "camera.h"

using namespace std;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    mCamera(NULL),
    mLayout(NULL),
    mLayout2(NULL)
{
    ui->setupUi(this);

    mCamera = new QCamera(this);
    mCameraViewfinder = new QCameraViewfinder(this);
    mLayout2 = new QVBoxLayout;
    mLayout = new QVBoxLayout;
    mCamera->start();

    mPart = new MyPartition();
    mCamera->setViewfinder(mCameraViewfinder);
    mLayout2->addWidget(mCameraViewfinder);
    mLayout2->setMargin(0);
    ui->frame_2->setLayout(mLayout2);
    mLayout->addWidget(mPart);
    ui->frame->setLayout(mLayout);

    ui->horizontalSlider_3->setValue(200);
    ui->horizontalSlider_4->setValue(200);

    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    foreach (const QCameraInfo &cameraInfo, cameras) {
        qDebug() << cameraInfo.description();
        ui->comboBox->addItem(cameraInfo.description());
    }
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_horizontalSlider_valueChanged(int value)
{
    mPart->setX(value *(630.0/200));
}

void Widget::on_horizontalSlider_2_valueChanged(int value)
{
    mPart->setY(value *(473.0/200));
}

void Widget::on_horizontalSlider_3_valueChanged(int value)
{
    mPart->setWidth(value*(630.0/200));
}

void Widget::on_horizontalSlider_4_valueChanged(int value)
{
    mPart->setHeight(value*(473.0/200));
}

void Widget::on_pushButton_released()
{
    mLaserTrack.setCrop(mPart->getX(), mPart->getY(), mPart->getWidth(), mPart->getHeight(), mPart->getMaxWidth(), mPart->getMaxHeight());
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    for(int i = 0; i < cameras.size(); i++)
    {
        if(cameras[i].deviceName() == QCameraInfo(*mCamera).deviceName())
        {
            mLaserTrack.setCameraIndex(i);
        }
    }
    delete mCamera;
    mLaserTrack.track();
}


void Widget::on_comboBox_currentIndexChanged(const QString &arg1)
{
    if(mCamera != NULL)
    {
        delete mCamera;
    }
    if(mLayout != NULL)
    {
        delete mLayout2;
    }
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    foreach (const QCameraInfo &cameraInfo, cameras) {
        if(cameraInfo.description() == arg1)
            mCamera = new QCamera(cameraInfo);
    }

    mLayout2 = new QVBoxLayout;
    mCamera->start();

    mCamera->setViewfinder(mCameraViewfinder);
    mLayout2->addWidget(mCameraViewfinder);
    mLayout2->setMargin(0);
    ui->frame_2->setLayout(mLayout2);
}
