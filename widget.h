#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QCameraInfo>
#include <QtCore>
#include <QtGui>
#include "mypartition.h"
#include "lasertrack.h"

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
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:

    void on_horizontalSlider_valueChanged(int value);

    void on_horizontalSlider_2_valueChanged(int value);

    void on_horizontalSlider_3_valueChanged(int value);

    void on_horizontalSlider_4_valueChanged(int value);

    void on_pushButton_released();

    void on_comboBox_currentIndexChanged(const QString &arg1);

private:
    Ui::Widget *ui;
    //codes begin
    QCamera *mCamera;
    QCameraViewfinder *mCameraViewfinder;
    QCameraImageCapture *mCameraImageCapture;
    QVBoxLayout *mLayout2;
    QVBoxLayout *mLayout;
    QFrame *mframe;
    MyPartition *mPart;
    LaserTrack mLaserTrack;
    //codes end
};

#endif // WIDGET_H
