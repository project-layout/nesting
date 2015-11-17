#include <math.h>
#include <ctype.h>
#include <string.h>
#include <algorithm>
#include <functional>

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

const char *GetLineNameStr(const Line &line)
{
    switch(line.type)
    {
    case LINE:
        return "LINE";
    case ARC:
        return "ARC";
    case CIRCLE:
        return "CIRCLE";
    default:
        return "UNKNOWN";
    }
}

std::string &LTrim(std::string &str)
{
    std::string::iterator it = std::find_if(str.begin(), str.end(), std::not1(std::ptr_fun<int, int>(isspace)));
    str.erase(str.begin(), it);
    return str;
}

std::string &RTrim(std::string &str)
{
    std::string::reverse_iterator it = std::find_if(str.rbegin(), str.rend(), std::not1(std::ptr_fun<int, int>(isspace)));
    str.erase(it.base(), str.end());
    return str;
}

std::string &Trim(std::string &str)
{
    return LTrim(RTrim(str));
}

char *LTrim(char *str)
{
    int len = strlen(str);
    char *p = std::find_if(&str[0], &str[len], std::not1(std::ptr_fun<int, int>(isspace))), *q = str;
    if(p != q)
    {
        while(*p != '\0')
            *q++ = *p++;
        *q = '\0';
    }
    return str;
}

char *RTrim(char *str)
{
    int len = strlen(str);
    int i = len-1;
    while(i >= 0 && isspace(str[i]))
    {
        i--;
    }
    str[i+1] = '\0';
    return str;
}

char *Trim(char *str)
{
    return LTrim(RTrim(str));
}
