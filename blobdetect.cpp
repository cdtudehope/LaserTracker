#include "blobdetect.h"
#include <iostream>

Blob::Blob(int x, int y)
{
    m_x.push_back(x);
    m_y.push_back(y);
}

void Blob::calcCentroid(int &centroidX, int &centroidY)
{
    int sum = 0;
    for(auto x_: m_x)
    {
        sum += x_;
    }
    centroidX = sum/(m_x.size());
    sum = 0;
    for(auto y_: m_y)
    {
        sum += y_;
    }
    centroidY = sum/(m_y.size());
}

void Blob::addToBlob(int x, int y)
{
    m_x.push_back(x);
    m_y.push_back(y);
}

//Checks if a point is near a blob
///Currently very inefficient code
bool Blob::isNear(int x, int y, int distanceThreshold)
{
    double distance;
    bool isNearBlob = false;
    for(int i = 0; i < m_x.size() && i < m_y.size(); i++)
    {
        distance = ((m_x[i] - x)*(m_x[i] - x))+((m_y[i] - y)*(m_y[i] - y));
        if(distance < distanceThreshold*distanceThreshold)
        {
            isNearBlob = true;
            break;
        }
    }
    return(isNearBlob);
}

//combines the passed in blob with the calling blob object
void Blob::combine(Blob& b)
{
    m_x.insert(m_x.end(), b.m_x.begin(), b.m_x.end());
    m_y.insert(m_y.end(), b.m_y.begin(), b.m_y.end());
}

BlobDetect::BlobDetect()
{
    m_blobs.clear();
}

//fills the blobs vector and returns the number of blobs
int BlobDetect::detect(cv::Mat img, int distanceThreshold)
{
    m_blobs.clear();
    m_dThres = distanceThreshold;
    for(int y=0; y<img.rows; y++)
    {
        for(int x=0; x<img.cols; x++)
        {
            if(img.at<unsigned char>(y,x) != 0) //If the pixel is not zero then it needs to be added to a blob or have a blob created for it.
            {
                add(x,y);
            }
        }
    }
    return(m_blobs.size());
}

void BlobDetect::add(int x, int y)
{
    if(m_blobs.size() == 0)
    {
        Blob b(x,y);
        m_blobs.push_back(b);
    }
    else
    {
        vector<int> nearBlobsIndex;
        for(int i = 0; i < m_blobs.size(); i++)
        {
            if(m_blobs[i].isNear(x,y,m_dThres))
            {
                nearBlobsIndex.push_back(i);
            }
        }

        //If the point is only near one blob then add to that blob
        if(nearBlobsIndex.size() == 1)
        {
            m_blobs[nearBlobsIndex[0]].addToBlob(x,y);
        }
        else if(nearBlobsIndex.size() > 1)  //if near more then one blob then combine the blobs and add to the new one
        {
            //Need to loop backwards so that I can delete blobs from the vector as I go without messing with the index of the other ones
            //This loop combines the list of blobs into the first blob in the list
            for(int i = nearBlobsIndex.size()-1; i<1; i--)
            {
                m_blobs[nearBlobsIndex[0]].combine(m_blobs[nearBlobsIndex[i]]);
                m_blobs.erase(m_blobs.begin() + nearBlobsIndex[i]);
            }
            m_blobs[nearBlobsIndex[0]].addToBlob(x,y);
        }
        else    //If no blobs found then make new one
        {
            Blob b(x,y);
            m_blobs.push_back(b);
        }
    }
}

//Returns by reference the centroid of the blob that is of the passed in index
//Returns false if index is out of range
bool BlobDetect::blobCentroid(int index, int& centroidX, int& centroidY)
{
    bool indexInRange = true;
    if(index < m_blobs.size())
    {
        m_blobs[index].calcCentroid(centroidX,centroidY);
    }
    else
    {
        centroidX = -1;
        centroidY = -1;
        indexInRange = false;
    }
    return(indexInRange);
}


