#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <sys/time.h>
#include <string>
#include <opencv2/opencv.hpp>
#include "blobdetect.h"
#include <Windows.h>
using namespace cv;
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    std::string path = "../LaserTracker/TestImages/";
    std::string imageName = "0";
    std::string ext = ".jpg";
    double elapsed_time, start_time;
    struct timeval tv;
    double min, max, maxMeanDiff;
    Point min_loc, max_loc;
    double averageTime = 0;
    BlobDetect detector;
    int numberOfBlobs = 0;
    for(int i = 0; i <34; i++)
    {
        imageName = "T" + std::to_string(i);
        // read an image
        gettimeofday(&tv, NULL);
        start_time = double(tv.tv_sec) + double(tv.tv_usec) * 0.000001;

        Mat image = imread(path + imageName + ext, CV_LOAD_IMAGE_COLOR);
        Mat bgr[3];
        Mat cropImage;
        Mat dst;
        Rect cropRectangle = Rect(172,122,520,278);
        Scalar meanRedPixelBrightness;
        cropImage = image(cropRectangle);
        imshow("My Image" + imageName, cropImage);
        gettimeofday(&tv, NULL);
        start_time = double(tv.tv_sec) + double(tv.tv_usec) * 0.000001;
        split(cropImage, bgr);
        meanRedPixelBrightness = mean(bgr[2]);
        minMaxLoc(bgr[2], &min, &max, &min_loc, &max_loc);
        maxMeanDiff = max - meanRedPixelBrightness[0];
        cout<<i<<"  "<<maxMeanDiff<<"   "<<meanRedPixelBrightness[0]<<"   "<<max<<"   "<<meanRedPixelBrightness[0]+(max-meanRedPixelBrightness[0])*0.96<<"  ";
        threshold(bgr[2], dst, meanRedPixelBrightness[0]+(max-meanRedPixelBrightness[0])*0.95, 255, THRESH_TOZERO);
        imshow("threashold" + imageName, dst);
        numberOfBlobs = detector.detect(dst);
        if(numberOfBlobs == 1)
        {
            int cx, cy;
            detector.blobCentroid(0, cx, cy);
            Point p(cx,cy);
            cout<<"Found a Laser"<<endl;
            circle(cropImage, p,5,Scalar(0,250,0));
            imshow("LastImageImage" + imageName, cropImage);
            int screenX = GetSystemMetrics(SM_CXSCREEN);
            int screenY = GetSystemMetrics(SM_CYSCREEN);
            int transformedX = cx*(double(screenX)/dst.cols);
            int transformedY = cy*(double(screenY)/dst.rows);
            SetCursorPos(transformedX, transformedY);
            //Sleep(5000);
        }
        else
        {
            cout<<"Found more then one blob in the image: "<<numberOfBlobs<<endl;
        }
        detector.clear();
        gettimeofday(&tv, NULL);
        elapsed_time = (double(tv.tv_sec) + double(tv.tv_usec) * 0.000001) - start_time;
        averageTime +=elapsed_time;
        cout<<elapsed_time<<endl;
    }
    averageTime = averageTime/34;
    cout<<"Average Time: "<<averageTime<<endl;
    cout<<"Average Frames per Second: "<<1/averageTime<<endl;

}

MainWindow::~MainWindow()
{
    delete ui;
}

