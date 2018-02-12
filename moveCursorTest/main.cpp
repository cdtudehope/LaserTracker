#include <iostream>
#include <Windows.h>
#include <math.h>
using namespace std;

void makeSquare();

int main()
{
    cout << "Hello World!" << endl;
    makeSquare();
    return 0;
}

void makeSquare()
{
    /*char stop = 'N';
    while(1)
    {
        //SetCursorPos(50*cos(i)+200, 50*sin(i)+200);
        //cout<<cos(i)+1000<<" "<<sin(i)+1000<<endl;
        POINT point;
        GetCursorPos(&point);
        cout<<point.x<<" "<<point.y<<endl;
    }*/
    int x = 200;
    int y = 200;
    Sleep(1000);
    while(x < 1079)
    {
        SetCursorPos(x, y);
        //Sleep(1);
        x++;
        cout<<x<<" "<<y<<endl;
    }
    while(y < 599)
    {
        SetCursorPos(x, y);
        //Sleep(1);
        y++;
        cout<<x<<" "<<y<<endl;
    }
    while(x > 200)
    {
        SetCursorPos(x, y);
        //Sleep(1);
        x--;
        cout<<x<<" "<<y<<endl;
    }
    while(y > 200)
    {
        SetCursorPos(x, y);
        //Sleep(1);
        y--;
        cout<<x<<" "<<y<<endl;
    }

}
