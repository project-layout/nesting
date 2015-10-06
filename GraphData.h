#ifndef GRAPHDATA_H
#define GRAPHDATA_H

#define PI 3.14159265358979


enum LineType
{
    LINE,
    ARC
};

struct Point
{
    double x, y;
};

struct Rect
{
    Point bl;   // bottom-left
    Point ur;   // upper-right
};

struct LineParam
{
    // empty
};

struct ArcParam
{
    double radian;
    int zDir;   // right-hand coordinate system
};

union LineParamUnion
{
    LineParam lineParam;
    ArcParam arcParam;
};

struct Line
{
    LineType type;
    LineParamUnion param;

    const char *GetLineNameStr()
    {
        switch(type)
        {
        case LINE:
            return "LINE";
        case ARC:
            return "ARC";
        default:
            return "UNKNOWN";
        }
    }
};

class Graph;

struct GraphInfo
{
    Graph *graph;
    int num;
};

#endif // GRAPHDATA_H
