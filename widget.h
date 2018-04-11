#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QCameraInfo>
#include <QtCore>
#include <QtGui>
#include "mypartition.h"
#include "lasertrack.h"
#include <dshow.h>
#include <oleauto.h>

namespace Ui {
class Widget;
}

//code begin
class QCamera;
class QCameraViewfinder;
class QCameraImageCapture;
class QVBoxLayout;
class QMenu;
class QAction;
class QFrame;
//code end

namespace Ui { class Widget; }

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(LaserTrack* lt, QWidget *parent = 0);
    ~Widget();

private slots:

    void on_horizontalSlider_valueChanged(int value);

    void on_horizontalSlider_2_valueChanged(int value);

    void on_horizontalSlider_3_valueChanged(int value);

    void on_horizontalSlider_4_valueChanged(int value);

    void on_pushButton_released();

    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_horizontalSlider_Exposure_valueChanged(int value);

    void on_horizontalSlider_Gain_valueChanged(int value);

    void on_horizontalSlider_Bright_valueChanged(int value);

    void on_horizontalSlider_Contrast_valueChanged(int value);

private:
    void setupCameraDShow(std::string cameraDiscription);
    void changeCameraToAuto();
    void changeCameraSliders();
    
    Ui::Widget *ui;

    QCamera *mCamera;
    QCameraViewfinder *mCameraViewfinder;
    QCameraImageCapture *mCameraImageCapture;
    QVBoxLayout *mCameraLayout;
    QVBoxLayout *mCropOverlayLayout;
    QFrame *mframe;
    MyPartition *mPart;
    LaserTrack* mLaserTrack;

    //DirectShow var
    IBaseFilter *mDeviceFilter;
};

#endif // WIDGET_H
