#ifndef MYPARTITION_H
#define MYPARTITION_H

#include <QWidget>
#include <QPainter>

class MyPartition : public QWidget
{
public:
    MyPartition(int width, int height);
    void paintEvent(QPaintEvent *event);

    //Using these setters will change the shape of the rectangle drawn
    int getXLeft();
    void setXLeft(int x);
    int getYTop();
    void setYTop(int y);
    int getXRight();
    void setXRight(int x);
    int getYBottom();
    void setYBottom(int y);

    //Width is calculated by right - left and bottom - top
    int getWidth();
    int getHeight();
    //Wdith set by constructor
    int getMaxWidth();
    int getMaxHeight();
private:
    QRect mRec;
    int mXLeft;
    int mYTop;
    int mXRight;
    int mYBottom;
    int maxWidth;
    int maxheight;
};

#endif // MYPARTITION_H
