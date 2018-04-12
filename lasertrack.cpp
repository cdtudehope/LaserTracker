#include "lasertrack.h"
#include <iostream>
#include <sys/time.h>
#include <string>
#include <opencv2/opencv.hpp>
#include <Windows.h>
#include <dshow.h>
#include <oleauto.h>
using namespace cv;
using namespace std;

LaserTrack::LaserTrack(): mcameraIndex(0)
{

}

void LaserTrack::track()
{
    VideoCapture cap(mcameraIndex + CAP_DSHOW);     //'+CAP_DSHOW' tells openCV to use Directshow as backend
    int i=0;
    if(!cap.isOpened())
    {
        cout<<"Video Capture object was not opened!"<<endl;
    }
    else
    {
        //Define all var up front so that they don't slow down the loop
        std::string imageName = "0";
        double elapsed_time, start_time;
        struct timeval tv;
        double min, max, maxMeanDiff;
        Point min_loc, max_loc;
        int numberOfBlobs = 0;
        Mat bgr[3];
        Mat image;
        Scalar meanRedPixelBrightness;
        Rect cropRectangle;
        bool leaveLoop = false;
        //If cap fails to read or bool var has been set to false, stop tracking
        while(leaveLoop != true && cap.read(image) == true)
        {
            i++;
            imageName = "T" + std::to_string(i);
            // read an image
            if(image.empty())
            {
              cout << "Something is wrong with the webcam, could not get frame." << endl;
              leaveLoop = true;
            }
            else
            {
                //image = imread(path + imageName + ext, CV_LOAD_IMAGE_COLOR);
                gettimeofday(&tv, NULL);
                start_time = double(tv.tv_sec) + double(tv.tv_usec) * 0.000001;
                cropRectangle = Rect(mCropX * image.cols,mCropY*image.rows,mCropWidth*image.cols,mCropHeight*image.rows);
                image = image(cropRectangle);
                //imshow("My Image" + imageName, cropImage);
                split(image, bgr);
                meanRedPixelBrightness = mean(bgr[1]);
                minMaxLoc(bgr[1], &min, &max, &min_loc, &max_loc);
                maxMeanDiff = max - meanRedPixelBrightness[0];
                cout<<i<<"  "<<maxMeanDiff<<"   "<<meanRedPixelBrightness[0]<<"   "<<max<<"   "<<meanRedPixelBrightness[0]+(max-meanRedPixelBrightness[0])*0.96<<"  ";
                threshold(bgr[1], image, meanRedPixelBrightness[0]+(max-meanRedPixelBrightness[0])*0.96, 255, THRESH_TOZERO);
                //imshow("threashold" + imageName, image);
                numberOfBlobs = mDetector.detect(image,4);
                if(numberOfBlobs == 1)
                {
                    int cx, cy;
                    mDetector.blobCentroid(0, cx, cy);
                    cout<<"Found a Laser"<<endl;
                    //imshow("LastImageImage" + imageName, image);
                    int screenX = GetSystemMetrics(SM_CXSCREEN);
                    int screenY = GetSystemMetrics(SM_CYSCREEN);
                    int transformedX = cx*(double(screenX)/image.cols);
                    int transformedY = cy*(double(screenY)/image.rows);
                    SetCursorPos(transformedX, transformedY);
                }
                else
                {
                    //imshow("My Image" + imageName, cropImage);
                    cout<<"Found more then one blob in the image: "<<numberOfBlobs<<endl;
                }
                gettimeofday(&tv, NULL);
                elapsed_time = (double(tv.tv_sec) + double(tv.tv_usec) * 0.000001) - start_time;
                cout<<elapsed_time<<endl;
            }
            if(isHotkey() == true)
            {
                cout<<"End Track Loop"<<endl;
                leaveLoop = true;
            }
        }
        cap.release();
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

void LaserTrack::setCameraName(string cameraName)
{
    mCameraName = cameraName;
}

bool LaserTrack::isHotkey()
{
    //Checks if control+WindowsKey+S is pressed on the keyboard
    return(GetAsyncKeyState(VK_CONTROL) && (GetAsyncKeyState(VK_LWIN) || GetAsyncKeyState(VK_RWIN)) && GetAsyncKeyState(0x53));
}
