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

using namespace std;

Widget::Widget(LaserTrack *lt, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    mCamera(NULL),
    mCropOverlayLayout(NULL),
    mCameraLayout(NULL),
    mLaserTrack(lt),
    mDeviceFilter(NULL)
{
    ui->setupUi(this);

    //Setup display of camera
    mCamera = new QCamera(this);
    mCameraViewfinder = new QCameraViewfinder(this);
    mCamera->start();

    mCameraLayout = new QVBoxLayout;
    mCamera->setViewfinder(mCameraViewfinder);
    mCameraLayout->addWidget(mCameraViewfinder);
    ui->cameraFrame->setLayout(mCameraLayout);

    //Setup an overlay that will draw over the camera image for croping
    mPart = new MyPartition();
    mCropOverlayLayout = new QVBoxLayout;
    mCropOverlayLayout->addWidget(mPart);
    ui->cropOverlayFrame->setLayout(mCropOverlayLayout);

    //Make sliders set to far right
    ui->horizontalSlider_3->setValue(200);
    ui->horizontalSlider_4->setValue(200);

    //Fill combobox with camera names
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    foreach (const QCameraInfo &cameraInfo, cameras)
    {
        qDebug() << cameraInfo.description();
        ui->comboBox->addItem(cameraInfo.description());
    }
}

Widget::~Widget()
{
    delete ui;
    delete mPart;
    mPart = NULL;

    //Need to change camera back to auto
    changeCameraToAuto();
}

void Widget::on_horizontalSlider_valueChanged(int value)
{
    mPart->setX(value *(630.0/200));    ///Need to change to something other then hard code for 630.0
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
    mLaserTrack->setCrop(mPart->getX(), mPart->getY(), mPart->getWidth(), mPart->getHeight(), mPart->getMaxWidth(), mPart->getMaxHeight());
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    for(int i = 0; i < cameras.size(); i++)
    {
        if(cameras[i].deviceName() == QCameraInfo(*mCamera).deviceName())
        {
            mLaserTrack->setCameraIndex(i);
            mLaserTrack->setCameraName(cameras[i].description().toStdString());
        }
    }
    delete mCamera;
    this->hide();
    mLaserTrack->track();
    this->close();
}


void Widget::on_comboBox_currentIndexChanged(const QString &arg1)
{
    //Reset camera settings back to default before closing it.
    changeCameraToAuto();

    //Clear old camera and layout pointers
    if(mCamera != NULL)
    {
        delete mCamera;
        mCamera = NULL;
    }
    if(mCameraLayout != NULL)
    {
        delete mCameraLayout;
        mCameraLayout = NULL;
    }

    //Set camera to currently selected option in combobox
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    foreach (const QCameraInfo &cameraInfo, cameras)
    {
        if(cameraInfo.description() == arg1)
        {
            mCamera = new QCamera(cameraInfo);
            setupCameraDShow(cameraInfo.description().toStdString()); //Sets up mDeviceFilter pointer
        }
    }

    //Change camera setting sliders to the new camera
    changeCameraSliders();

    mCameraLayout = new QVBoxLayout;
    mCamera->start();
    mCamera->setViewfinder(mCameraViewfinder);
    mCameraLayout->addWidget(mCameraViewfinder);
    ui->cameraFrame->setLayout(mCameraLayout);
}

//Sets up the directShow filter (mDeviceFilter) to point to the camera with the passed in name
void Widget::setupCameraDShow(string cameraDiscription)
{
    if(mDeviceFilter != NULL)
    {
        mDeviceFilter->Release();
        mDeviceFilter = NULL;
    }
    // initialize COM
    CoInitialize(NULL);
    ICreateDevEnum *createDevEnum = NULL;
    // Create CreateDevEnum to list device
    CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, (PVOID *)&createDevEnum);

    // Create EnumMoniker to list VideoInputDevice
    IEnumMoniker *enumMoniker = NULL;
    createDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &enumMoniker, 0);
    if (enumMoniker == NULL) {
        // this will be shown if there is no capture device
        cout<<"no device"<<endl;
    }
    else
    {
        // reset EnumMoniker
        enumMoniker->Reset();

        bool cameraFound = false;
        IMoniker *moniker = NULL;
        ULONG fetched = 0;
        // get each Moniker
        while (enumMoniker->Next(1, &moniker, &fetched) == S_OK && cameraFound != true)
        {
            IPropertyBag *propertyBag;
            char devname[256];

            // bind to IPropertyBag
            moniker->BindToStorage(0, 0, IID_IPropertyBag, (void **)&propertyBag);

            // get FriendlyName
            VARIANT var;
            var.vt = VT_BSTR;
            propertyBag->Read(L"FriendlyName",&var,0);
            WideCharToMultiByte(CP_ACP, 0, var.bstrVal, -1, devname, sizeof(devname), 0, 0);
            VariantClear(&var);
            string tempCameraName = string(devname);
            cout<<tempCameraName<<endl;

            if (tempCameraName == cameraDiscription)
            {
                // Bind Monkier to Filter
                moniker->BindToObject(0, 0, IID_IBaseFilter, (void**)&mDeviceFilter);
                cameraFound = true;
            }

            // release
            moniker->Release();
            propertyBag->Release();
        }
    }
    // finalize COM
    CoUninitialize();
}

//Sets all of the modified camera setting back to auto and default settings
void Widget::changeCameraToAuto()
{
    if (mDeviceFilter != NULL)
    {
        //Exposure
        HRESULT hr = CoInitialize(0);
        //Query the capture filter for the IAMCameraControl interface.
        IAMCameraControl *cameraControl = 0;
        hr = mDeviceFilter->QueryInterface(IID_IAMCameraControl, (void**)&cameraControl);
        if (FAILED(hr))
        {
            // The device does not support IAMCameraControl
        }
        else
        {
            long Min, Max, Step, Default, Flags;
            // Get the range and default values
            hr = cameraControl->GetRange(CameraControl_Exposure, &Min, &Max, &Step, &Default, &Flags);
            if (SUCCEEDED(hr))
            {
                if(Flags != CameraControl_Flags_Auto)
                {
                    hr = cameraControl->Set(CameraControl_Exposure,Default,CameraControl_Flags_Auto);
                    if(FAILED(hr))
                    {   //It may fail if CameraControl_Flags_Auto is not avalable
                        //Try same with CameraControl_Flags_Manuel
                        hr = cameraControl->Set(CameraControl_Exposure,Default,CameraControl_Flags_Manual);
                        if(FAILED(hr))
                        {   //If this fails there is no hope
                            cout<<"Failed to reset exposure!"<<endl;
                        }
                    }
                }
            }
        }

        // Query the capture filter for the IAMVideoProcAmp interface.
        IAMVideoProcAmp *procAmp = 0;
        hr = mDeviceFilter->QueryInterface(IID_IAMVideoProcAmp, (void**)&procAmp);
        if (FAILED(hr))
        {
            // The device does not support IAMVideoProcAmp
        }
        else
        {
            long Min, Max, Step, Default, Flags;
            // Get the range and default values
            hr = procAmp->GetRange(VideoProcAmp_Brightness, &Min, &Max, &Step, &Default, &Flags);
            if (SUCCEEDED(hr))
            {
                if(Flags != CameraControl_Flags_Auto)
                {
                    hr = procAmp->Set(VideoProcAmp_Brightness, Default, CameraControl_Flags_Auto);
                    if(FAILED(hr))
                    {   //It may fail if CameraControl_Flags_Auto is not avalable
                        //Try same with CameraControl_Flags_Manuel
                        hr = procAmp->Set(VideoProcAmp_Brightness, Default, CameraControl_Flags_Manual);
                        if(FAILED(hr))
                        {   //If this fails there is no hope
                            cout<<"Failed to reset brightness!"<<endl;
                        }
                    }
                }
            }

            hr = procAmp->GetRange(VideoProcAmp_Contrast, &Min, &Max, &Step, &Default, &Flags);
            if (SUCCEEDED(hr))
            {
                if(Flags != CameraControl_Flags_Auto)
                {
                    hr = procAmp->Set(VideoProcAmp_Contrast, Default, CameraControl_Flags_Auto);
                    if(FAILED(hr))
                    {   //It may fail if CameraControl_Flags_Auto is not avalable
                        //Try same with CameraControl_Flags_Manuel
                        hr = procAmp->Set(VideoProcAmp_Contrast, Default, CameraControl_Flags_Manual);
                        if(FAILED(hr))
                        {   //If this fails there is no hope
                            cout<<"Failed to reset contrast!"<<endl;
                        }
                    }
                }
            }

            hr = procAmp->GetRange(VideoProcAmp_Gain, &Min, &Max, &Step, &Default, &Flags);
            if (SUCCEEDED(hr))
            {
                if(Flags != CameraControl_Flags_Auto)
                {
                    hr = procAmp->Set(VideoProcAmp_Gain, Default, CameraControl_Flags_Auto);
                    if(FAILED(hr))
                    {   //It may fail if CameraControl_Flags_Auto is not avalable
                        //Try same with CameraControl_Flags_Manuel
                        hr = procAmp->Set(VideoProcAmp_Gain, Default, CameraControl_Flags_Manual);
                        if(FAILED(hr))
                        {   //If this fails there is no hope
                            cout<<"Failed to reset gain!"<<endl;
                        }
                    }
                }
            }
        }
    }
}

void Widget::changeCameraSliders()
{
    if (mDeviceFilter != NULL)
    {
        //Exposure
        HRESULT hr = CoInitialize(0);
        //Query the capture filter for the IAMCameraControl interface.
        IAMCameraControl *cameraControl = 0;
        hr = mDeviceFilter->QueryInterface(IID_IAMCameraControl, (void**)&cameraControl);
        if (FAILED(hr))
        {
            // The device does not support IAMCameraControl
        }
        else
        {
            long Min, Max, Step, Default, Flags, Val;
            // Get the range and default values
            hr = cameraControl->GetRange(CameraControl_Exposure, &Min, &Max, &Step, &Default, &Flags);
            cout<<"Exposure>Max: "<<Max<<" Min: "<<Min<<" Step: "<<Step<<endl;
            if (SUCCEEDED(hr))
            {
                //Need to call get first because setting sliders will cause a change reaction that will change the cameras value
                hr = cameraControl->Get(CameraControl_Exposure,&Val,&Flags);
                ui->horizontalSlider_Exposure->setTickInterval(Step);
                ui->horizontalSlider_Exposure->setMaximum(Max);
                ui->horizontalSlider_Exposure->setMinimum(Min);
                if (SUCCEEDED(hr))
                {
                    ui->horizontalSlider_Exposure->setValue(Val);
                }
            }
        }

        // Query the capture filter for the IAMVideoProcAmp interface.
        IAMVideoProcAmp *procAmp = 0;
        hr = mDeviceFilter->QueryInterface(IID_IAMVideoProcAmp, (void**)&procAmp);
        if (FAILED(hr))
        {
            // The device does not support IAMVideoProcAmp
        }
        else
        {
            long Min, Max, Step, Default, Flags, Val;
            // Get the range and default values
            hr = procAmp->GetRange(VideoProcAmp_Brightness, &Min, &Max, &Step, &Default, &Flags);
            cout<<"Brightness>Max: "<<Max<<" Min: "<<Min<<" Step: "<<Step<<endl;
            if (SUCCEEDED(hr))
            {
                //Need to call get first because setting sliders will cause a change reaction that will change the cameras value
                hr = procAmp->Get(VideoProcAmp_Brightness, &Val, &Flags);
                ui->horizontalSlider_Bright->setTickInterval(Step);
                ui->horizontalSlider_Bright->setMaximum(Max);
                ui->horizontalSlider_Bright->setMinimum(Min);
                if (SUCCEEDED(hr))
                {
                    ui->horizontalSlider_Bright->setValue(Val);
                }
            }

            hr = procAmp->GetRange(VideoProcAmp_Contrast, &Min, &Max, &Step, &Default, &Flags);
            cout<<"Contrast>Max: "<<Max<<" Min: "<<Min<<" Step: "<<Step<<endl;
            if (SUCCEEDED(hr))
            {
                //Need to call get first because setting sliders will cause a change reaction that will change the cameras value
                hr = procAmp->Get(VideoProcAmp_Contrast, &Val, &Flags);
                ui->horizontalSlider_Contrast->setTickInterval(Step);
                ui->horizontalSlider_Contrast->setMaximum(Max);
                ui->horizontalSlider_Contrast->setMinimum(Min);
                if (SUCCEEDED(hr))
                {
                    ui->horizontalSlider_Contrast->setValue(Val);
                }
            }

            hr = procAmp->GetRange(VideoProcAmp_Gain, &Min, &Max, &Step, &Default, &Flags);
            cout<<"Gain>Max: "<<Max<<" Min: "<<Min<<" Step: "<<Step<<endl;
            if (SUCCEEDED(hr))
            {
                //Need to call get first because setting sliders will cause a change reaction that will change the cameras value
                hr = procAmp->Get(VideoProcAmp_Gain, &Val, &Flags);
                ui->horizontalSlider_Gain->setTickInterval(Step);
                ui->horizontalSlider_Gain->setMaximum(Max);
                ui->horizontalSlider_Gain->setMinimum(Min);
                if (SUCCEEDED(hr))
                {
                    ui->horizontalSlider_Gain->setValue(Val);
                }
            }
        }
    }    
}

void Widget::on_horizontalSlider_Exposure_valueChanged(int value)
{
    if (mDeviceFilter != NULL)
    {
        //Exposure
        HRESULT hr = CoInitialize(0);
        //Query the capture filter for the IAMCameraControl interface.
        IAMCameraControl *cameraControl = 0;
        hr = mDeviceFilter->QueryInterface(IID_IAMCameraControl, (void**)&cameraControl);
        if (FAILED(hr))
        {
            cout<<"The device does not support IAMCameraControl"<<endl;
        }
        else
        {
            hr = cameraControl->Set(CameraControl_Exposure, value, CameraControl_Flags_Manual);
            if (FAILED(hr))
            {
                cout<<"Failed to change exposure!"<<endl;
            }
        }
        // finalize COM
        CoUninitialize();
    }
    else
    {
        cout<<"Error: DShow not set up"<<endl;
    }
}

void Widget::on_horizontalSlider_Gain_valueChanged(int value)
{
    if (mDeviceFilter != NULL)
    {
        //Exposure
        HRESULT hr = CoInitialize(0);
        //Query the capture filter for the IAMVideoProcAmp interface.
        IAMVideoProcAmp *procAmp = 0;
        hr = mDeviceFilter->QueryInterface(IID_IAMVideoProcAmp, (void**)&procAmp);
        if (FAILED(hr))
        {
            cout<<"The device does not support IAMVideoProcAmp"<<endl;
        }
        else
        {
            hr = procAmp->Set(VideoProcAmp_Gain, value, CameraControl_Flags_Manual);
            if (FAILED(hr))
            {
                cout<<"Failed to change Gain!"<<endl;
            }
        }
        // finalize COM
        CoUninitialize();
    }
    else
    {
        cout<<"Error: DShow not set up"<<endl;
    }
}

void Widget::on_horizontalSlider_Bright_valueChanged(int value)
{
    if (mDeviceFilter != NULL)
    {
        //Exposure
        HRESULT hr = CoInitialize(0);
        //Query the capture filter for the IAMVideoProcAmp interface.
        IAMVideoProcAmp *procAmp = 0;
        hr = mDeviceFilter->QueryInterface(IID_IAMVideoProcAmp, (void**)&procAmp);
        if (FAILED(hr))
        {
            cout<<"The device does not support IAMVideoProcAmp"<<endl;
        }
        else
        {
            hr = procAmp->Set(VideoProcAmp_Brightness, value, CameraControl_Flags_Manual);
            if (FAILED(hr))
            {
                cout<<"Failed to change Brightness!"<<endl;
            }
        }
        // finalize COM
        CoUninitialize();
    }
    else
    {
        cout<<"Error: DShow not set up"<<endl;
    }
}

void Widget::on_horizontalSlider_Contrast_valueChanged(int value)
{
    if (mDeviceFilter != NULL)
    {
        //Exposure
        HRESULT hr = CoInitialize(0);
        //Query the capture filter for the IAMVideoProcAmp interface.
        IAMVideoProcAmp *procAmp = 0;
        hr = mDeviceFilter->QueryInterface(IID_IAMVideoProcAmp, (void**)&procAmp);
        if (FAILED(hr))
        {
            cout<<"The device does not support IAMVideoProcAmp"<<endl;
        }
        else
        {
            hr = procAmp->Set(VideoProcAmp_Contrast, value, CameraControl_Flags_Manual);
            if (FAILED(hr))
            {
                cout<<"Failed to change Contrast!"<<endl;
            }
        }
        // finalize COM
        CoUninitialize();
    }
    else
    {
        cout<<"Error: DShow not set up"<<endl;
    }
}
