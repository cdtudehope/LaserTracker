#include "mypartition.h"
#include "iostream"

MyPartition::MyPartition(int width, int height):
    maxWidth(width), maxheight(height),
    mXLeft(0), mYTop(0), mXRight(width), mYBottom(height)
{}

void MyPartition::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    mRec.setRect(mXLeft,mYTop,getWidth()-21,getHeight()-20);    //The subtractions is there because it was acting weird
    QPen pen(Qt::blue);
    pen.setWidth(3);
    painter.setPen(pen);
    painter.drawRect(mRec);
}

int MyPartition::getXLeft()
{
    return(mXLeft);
}

void MyPartition::setXLeft(int x)
{
    mXLeft = x;
}

int MyPartition::getYTop()
{
    return(mYTop);
}

void MyPartition::setYTop(int y)
{
    mYTop = y;
}

int MyPartition::getXRight()
{
    return(mXRight);
}

void MyPartition::setXRight(int x)
{
    mXRight = x;
}

int MyPartition::getYBottom()
{
    return(mYBottom);
}

void MyPartition::setYBottom(int y)
{
    mYBottom = y;
}

int MyPartition::getWidth()
{
    return(mXRight-mXLeft);
}

int MyPartition::getHeight()
{
    return(mYBottom-mYTop);
}

int MyPartition::getMaxWidth()
{
    return(maxWidth);
}

int MyPartition::getMaxHeight()
{
    return(maxheight);
}

