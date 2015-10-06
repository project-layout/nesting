#include <math.h>

#include "Utils.h"


Point GetCircleCenter(const Point &p1, const Point &p2, double radian, int zDir)
{
    double x1 = p1.x, x2 = p2.x, y1 = p1.y, y2 = p2.y;
    double chordLen = sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
    double radius = chordLen/2/sin(radian/2);
    double theta;   // the angle between CP1 and x-axis
    double gamma = atan2(y2-y1, x2-x1);   // the angle between P1P2 and x-axis
    Point cc;
    if(x1 <= x2 && y1 <= y2)
    {
        if(zDir > 0)
        {
            theta = -(PI+radian)/2+gamma;
        }
        else
        {
            theta = (PI+radian)/2+gamma;
        }
    }
    else if(x1 >= x2 && y1 >= y2)
    {
        if(zDir > 0)
        {
            theta = (3*PI-radian)/2+gamma;
        }
        else
        {
            theta = (PI+radian)/2+gamma;
        }
    }
    else if(x1 <= x2 && y1 >= y2)
    {
        if(zDir > 0)
        {
            theta = -(PI+radian)/2+gamma;
        }
        else
        {
            theta = (PI+radian)/2+gamma;
        }
    }
    else if(x1 >= x2 && y1 <= y2)
    {
        if(zDir > 0)
        {
            theta = -(PI+radian)/2+gamma;
        }
        else
        {
            theta = -(3*PI-radian)/2+gamma;
        }
    }
    cc.x = x1-radius*cos(theta);
    cc.y = y1-radius*sin(theta);
    return cc;
}
