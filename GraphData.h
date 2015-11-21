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
    Point center;
    double radius;
    double startAng, endAng;
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

struct NestingResult
{
    int board;
    Point pos;
    double angle;
    const Graph *graph;
};

#endif // GRAPHDATA_H
