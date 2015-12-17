#ifndef GRAPHDATA_H
#define GRAPHDATA_H

struct Point
{
    double x, y;
};

struct Rect
{
    Point bl;   // bottom-left
    Point ur;   // upper-right
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
    const Graph *graph;
    double angle;
};

#endif // GRAPHDATA_H
