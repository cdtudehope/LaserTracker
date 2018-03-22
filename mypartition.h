#ifndef MYPARTITION_H
#define MYPARTITION_H

#include <QWidget>
#include <QPainter>

class MyPartition : public QWidget
{
public:
    MyPartition();
    void paintEvent(QPaintEvent *event);
    void decBox();
    int getX();
    void setX(int x);
    int getY();
    void setY(int y);
    int getWidth();
    void setWidth(int w);
    int getHeight();
    void setHeight(int h);
    int getMaxWidth();
    int getMaxHeight();
private:
    QRect rec;
    int mX;
    int mY;
    int mWidth;
    int mHeight;
    int maxWidth;
    int maxHeight;
};

#endif // MYPARTITION_H
