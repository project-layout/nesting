#ifndef GRAPHDATA_H
#define GRAPHDATA_H

#define PI 3.14159265358979


enum LineType
{
    LINE,
    ARC,
    CIRCLE
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
    Point ep1, ep2;
};

struct ArcParam
{
    Point ep1, ep2;
    double radian;
    int zDir;   // right-hand coordinate system
};

struct CircleParam
{
    Point center;
    double radius;
};

union LineParamUnion
{
    LineParam lineParam;
    ArcParam arcParam;
    CircleParam circleParam;
};

struct Line
{
    LineType type;
    LineParamUnion param;
};

class Graph;

struct GraphInfo
{
    Graph *graph;
    int num;
};

#endif // GRAPHDATA_H
