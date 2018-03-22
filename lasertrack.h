#ifndef LASERTRACK_H
#define LASERTRACK_H


class LaserTrack
{
public:
    LaserTrack();
    void track();
    void setCrop(int x, int y, int width, int height, const int maxWidth, const int maxHeight);
    void setCameraIndex(int index);
private:
    double mCropX;
    double mCropY;
    double mCropWidth;
    double mCropHeight;
    int mcameraIndex;
};

#endif // LASERTRACK_H
