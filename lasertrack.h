#ifndef LASERTRACK_H
#define LASERTRACK_H
#include <string>
#include "blobdetect.h"

class LaserTrack
{
public:
    LaserTrack();
    void track();
    void setCrop(int x, int y, int width, int height, const int maxWidth, const int maxHeight);
    void setCameraIndex(int index);
    void setCameraName(std::string cameraName);
private:
    bool isHotkey();
    double mCropX;
    double mCropY;
    double mCropWidth;
    double mCropHeight;
    int mcameraIndex;
    std::string mCameraName;
    BlobDetect mDetector;
};

#endif // LASERTRACK_H
