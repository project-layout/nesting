#include <stdio.h>

#include "GraphOutline.h"

GraphOutline::GraphOutline()
{
    //ctor
}

GraphOutline::~GraphOutline()
{
    //dtor
}

int GraphOutline::AddPoint(double x, double y)
{
    Point p;
    p.x = x;
    p.y = y;
    pointSet.push_back(p);
    return pointSet.size()-1;
}

int GraphOutline::AddLine(LineType type, LineParamUnion param)
{
    Line line;
    line.type = type;
    line.param = param;
    lineSet.push_back(line);
    return lineSet.size()-1;
}

void GraphOutline::Print()
{
    int i;
    printf("The number of points: %d\n", pointSet.size());
    for(i = 0; i < (int)pointSet.size(); i++)
    {
        printf("  Point %d: (%lf,%lf)\n", i, pointSet[i].x, pointSet[i].y);
    }
    for(i = 0; i < (int)lineSet.size(); i++)
    {
        printf("  Line %d->%d: %s ", i, (i+1)%lineSet.size(), lineSet[i].GetLineNameStr());
        switch(lineSet[i].type)
        {
        case LINE:
            printf("\n");
            break;
        case ARC:
            printf("radian=%lf dir=%d\n", lineSet[i].param.arcParam.radian, lineSet[i].param.arcParam.zDir);
            break;
        default:
            printf("\n");
        }
    }
}
