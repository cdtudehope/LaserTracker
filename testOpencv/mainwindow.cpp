#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <sys/time.h>
#include <string>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    std::string path = "C:/Users/Chris\ Tudehope/Pictures/Camera\ Roll/";
    std::string imageName = "0";
    std::string ext = ".jpg";
    double elapsed_time, start_time;
    struct timeval tv;
    double min, max;
    Point min_loc, max_loc;
    double averageTime = 0;
    for(int i = 0; i < 6; i++)
    {
        if(i != 2)
        {
            imageName = std::to_string(i) + "p";
            // read an image
            gettimeofday(&tv, NULL);
            start_time = double(tv.tv_sec) + double(tv.tv_usec) * 0.000001;

            Mat image = imread(path + imageName + ext, CV_LOAD_IMAGE_COLOR);
            Mat bgr[3];
            Mat cropImage;
            Mat dst;
            Rect cropRectangle = Rect(177,233,478,257);
            Scalar meanRedPixelBrightness;
            cropImage = image(cropRectangle);
            imshow("My Image" + imageName, cropImage);
            gettimeofday(&tv, NULL);
            start_time = double(tv.tv_sec) + double(tv.tv_usec) * 0.000001;
            split(cropImage, bgr);
            meanRedPixelBrightness = mean(bgr[2]);
            minMaxLoc(bgr[2], &min, &max, &min_loc, &max_loc);
            cout<<i<<"  "<<meanRedPixelBrightness[0]+(max-meanRedPixelBrightness[0])*0.94<<"  ";
            threshold(bgr[2], dst, meanRedPixelBrightness[0]+(max-meanRedPixelBrightness[0])*0.94, 255, THRESH_TOZERO);
            imshow("threashold" + imageName, dst);

            gettimeofday(&tv, NULL);
            elapsed_time = (double(tv.tv_sec) + double(tv.tv_usec) * 0.000001) - start_time;
            averageTime +=elapsed_time;
            cout<<elapsed_time<<endl;
        }
    }
    averageTime = averageTime/5;
    cout<<"Average Time: "<<averageTime<<endl;
    cout<<"Average Frames per Second: "<<1/averageTime<<endl;

}

MainWindow::~MainWindow()
{
    delete ui;
}
