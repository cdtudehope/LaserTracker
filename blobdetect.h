#ifndef BLOBDETECT_H
#define BLOBDETECT_H

#include <vector>
#include "opencv2/core/mat.hpp"
using namespace std;

//Used to store information about each blob
class Blob
{
public:
    Blob(int x,int y);
    void calcCentroid(int& centroidX, int& centroidY);
    void addToBlob(int x, int y);
    bool isNear(int x, int y, int distanceThreshold);
    void combine(Blob& b);
private:
    vector<int> m_x;
    vector<int> m_y;
};

class BlobDetect
{
public:
    BlobDetect();
    //This function will assume that the image has already been threasholded and that it is
    //a blacked out image with white pixels
    int detect(cv::Mat img, int distanceThreshold);
    bool blobCentroid(int index, int& x, int& y);
private:
    bool isNear(int x, int y);
    void add(int x, int y);
    int m_dThres;
    vector<Blob> m_blobs;
};

#endif // BLOBDETECT_H
