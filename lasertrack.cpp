#include "lasertrack.h"
#include <iostream>
#include <sys/time.h>
#include <string>
#include <opencv2/opencv.hpp>
#include "blobdetect.h"
#include <Windows.h>
using namespace cv;
using namespace std;

LaserTrack::LaserTrack(): mcameraIndex(0)
{

}

void LaserTrack::track()
{
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
    Mat bgr[3];
    Mat cropImage;
    Mat dst;
    Mat image;
    Scalar meanRedPixelBrightness;
    Rect cropRectangle;
    VideoCapture cap(0);
    int i=0;
    if(!cap.isOpened())
    {
        cout<<"Video Capture object was not opened!"<<endl;
    }
    else
    {
        for(;;)
        {
            i++;
            imageName = "T" + std::to_string(i);
            // read an image
            gettimeofday(&tv, NULL);
            start_time = double(tv.tv_sec) + double(tv.tv_usec) * 0.000001;
            cap >> image;
            if(image.empty())
            {
              cout << "Something is wrong with the webcam, could not get frame." << endl;
            }
            else
            {
                //image = imread(path + imageName + ext, CV_LOAD_IMAGE_COLOR);
                gettimeofday(&tv, NULL);
                start_time = double(tv.tv_sec) + double(tv.tv_usec) * 0.000001;
                cropRectangle = Rect(mCropX * image.cols,mCropY*image.rows,mCropWidth*image.cols,mCropHeight*image.rows);
                cropImage = image(cropRectangle);
                //imshow("My Image" + imageName, cropImage);
                split(cropImage, bgr);
                meanRedPixelBrightness = mean(bgr[2]);
                minMaxLoc(bgr[2], &min, &max, &min_loc, &max_loc);
                maxMeanDiff = max - meanRedPixelBrightness[0];
                cout<<i<<"  "<<maxMeanDiff<<"   "<<meanRedPixelBrightness[0]<<"   "<<max<<"   "<<meanRedPixelBrightness[0]+(max-meanRedPixelBrightness[0])*0.96<<"  ";
                threshold(bgr[2], dst, meanRedPixelBrightness[0]+(max-meanRedPixelBrightness[0])*0.96, 255, THRESH_TOZERO);
                imshow("threashold" + imageName, dst);
                numberOfBlobs = detector.detect(dst,2);
                if(numberOfBlobs == 1)
                {
                    int cx, cy;
                    detector.blobCentroid(0, cx, cy);
                    cout<<"Found a Laser"<<endl;
                    circle(cropImage, Point(cx,cy),5,Scalar(0,250,0));
                    //imshow("LastImageImage" + imageName, cropImage);
                    int screenX = GetSystemMetrics(SM_CXSCREEN);
                    int screenY = GetSystemMetrics(SM_CYSCREEN);
                    int transformedX = cx*(double(screenX)/dst.cols);
                    int transformedY = cy*(double(screenY)/dst.rows);
                    SetCursorPos(transformedX, transformedY);
                    //Sleep(5000);
                }
                else
                {
                    //imshow("My Image" + imageName, cropImage);
                    cout<<"Found more then one blob in the image: "<<numberOfBlobs<<endl;
                }
            }
            gettimeofday(&tv, NULL);
            elapsed_time = (double(tv.tv_sec) + double(tv.tv_usec) * 0.000001) - start_time;
            averageTime +=elapsed_time;
            cout<<elapsed_time<<endl;
        }
        averageTime = averageTime/34;
        cout<<"Average Time: "<<averageTime<<endl;
        cout<<"Average Frames per Second: "<<1/averageTime<<endl;
    }

}

void LaserTrack::setCrop(int x, int y, int width, int height, const int maxWidth, const int maxHeight)
{
    //Will need to multiply by the resolution when used
    mCropX = double(x) /maxWidth;
    mCropY = double(y) /maxHeight;
    mCropWidth = double(width)/maxWidth;
    mCropHeight = double(height)/maxHeight;
}

void LaserTrack::setCameraIndex(int index)
{
    mcameraIndex = index;
}
