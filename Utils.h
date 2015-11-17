#ifndef UTILS_H
#define UTILS_H

#include <string>

#include "GraphData.h"


Point GetCircleCenter(const Point &p1, const Point &p2, double radian, int zDir);

inline bool FloatEqual(double x, double y, double precision)
{
    return x - y < precision && y - x < precision;
}

inline bool FloatLess(double x, double y, double precision)
{
    return y - x >= precision;
}

inline bool FloatGreater(double x, double y, double precision)
{
    return x - y >= precision;
}

const char *GetLineNameStr(const Line &line);

std::string &LTrim(std::string &str);
std::string &RTrim(std::string &str);
std::string &Trim(std::string &str);
char *LTrim(char *str);
char *RTrim(char *str);
char *Trim(char *str);

#endif // UTILS_H
