#include "mypartition.h"
#include "iostream"

MyPartition::MyPartition()
{
    mWidth = maxWidth = 630;
    mHeight = maxHeight = 473;
    mX=1;
    mY=1;
}

void MyPartition::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    rec.setRect(mX,mY,mWidth,mHeight);
    QPen pen(Qt::blue);
    pen.setWidth(3);
    painter.setPen(pen);
    painter.drawRect(rec);
}

void MyPartition::decBox()
{
    mX = rec.x() + 10;
    mY = rec.y() + 10;
    mWidth = rec.width() - 20;
    mHeight = rec.height() - 20;
}

int MyPartition::getX()
{
    return(mX);
}

void MyPartition::setX(int x)
{
    mWidth += (mX - x);
    mX = x;
}

int MyPartition::getY()
{
    return(mY);
}

void MyPartition::setY(int y)
{
    mHeight += (mY - y);
    mY = y;
}

int MyPartition::getWidth()
{
    return(mWidth);
}

void MyPartition::setWidth(int w)
{
    mWidth = w;
}

int MyPartition::getHeight()
{
    return(mHeight);
}

void MyPartition::setHeight(int h)
{
    mHeight = h;
}

int MyPartition::getMaxWidth()
{
    return(maxWidth);
}

int MyPartition::getMaxHeight()
{
    return(maxHeight);
}

