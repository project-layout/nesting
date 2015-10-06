#include <stdio.h>
#include <math.h>
#include <float.h>

#include "SimpleLayouter.h"
#include "Graph.h"
#include "Utils.h"


SimpleLayouter::SimpleLayouter(const std::vector<GraphInfo> &graphSet, Point boardSize, std::vector<LayoutResult> &result, double precision)
    : graphSet(graphSet)
    , boardSize(boardSize)
    , precision(precision)
    , result(result)
{
    enclosureRect.reserve(graphSet.size());
}

SimpleLayouter::~SimpleLayouter()
{
    //dtor
}

void SimpleLayouter::Solve()
{
    int i;
    Rect rect;
    for(i = 0; i < (int)graphSet.size(); i++)
    {
        EnclosureRect(graphSet[i].graph, &rect);
        enclosureRect.push_back(rect);
    }
    PlaceOnBoard();
}

void SimpleLayouter::EnclosureRect(const Graph *graph, Rect *rect)
{
    int i, j;
    GraphOutline *outline;
    Rect lineRect;
    rect->ur.x = rect->ur.y = -(rect->bl.x = rect->bl.y = DBL_MAX);
    for(i = 0; i < graph->GetOutlineNum(); i++)
    {
        outline = graph->GetOutline(i);
        if(outline == NULL)
        {
            printf("Error: outline is null\n"); // should not be here
            continue;
        }
        for(j = 0; j < outline->GetPointNum(); j++)
        {
            Point *point1 = outline->GetPoint(j);
            Point *point2 = outline->GetPoint((j+1)%outline->GetPointNum());
            Line *line = outline->GetLine(j);
            switch(line->type)
            {
            case LINE:
                EnclosureRectForLine(line, point1, point2, &lineRect);
                break;
            case ARC:
                EnclosureRectForArc(line, point1, point2, &lineRect);
                break;
            }
            if(lineRect.bl.x < rect->bl.x)
                rect->bl.x = lineRect.bl.x;
            if(lineRect.bl.y < rect->bl.y)
                rect->bl.y = lineRect.bl.y;
            if(lineRect.ur.x > rect->ur.x)
                rect->ur.x = lineRect.ur.x;
            if(lineRect.ur.y > rect->ur.y)
                rect->ur.y = lineRect.ur.y;
        }
    }
}

void SimpleLayouter::EnclosureRectForLine(const Line *line, const Point *p1, const Point *p2, Rect *rect)
{
    double x1 = p1->x, x2 = p2->x, y1 = p1->y, y2 = p2->y;
    rect->bl.x = x1 < x2 ? x1 : x2;
    rect->bl.y = y1 < y2 ? y1 : y2;
    rect->ur.x = x1 > x2 ? x1 : x2;
    rect->ur.y = y1 > y2 ? y1 : y2;
}

void SimpleLayouter::EnclosureRectForArc(const Line *line, const Point *p1, const Point *p2, Rect *rect)
{
    double x1 = p1->x, x2 = p2->x, y1 = p1->y, y2 = p2->y;
    double radian = line->param.arcParam.radian;
    double chordLen = sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
    double radius = chordLen/2/sin(radian/2);
    int zDir = line->param.arcParam.zDir;
    Point pc = GetCircleCenter(*p1, *p2, radian, zDir);

    double angle1 = atan2(p1->y-pc.y, p1->x-pc.x);
    double angle2 = atan2(p2->y-pc.y, p2->x-pc.x);
    double angle, x, y;
    rect->ur.x = rect->ur.y = -(rect->bl.x = rect->bl.y = DBL_MAX);
    for(angle = angle1; angle < angle2+EPS; angle += precision)
    {
        x = pc.x+radius*cos(angle);
        y = pc.y+radius*sin(angle);
        if(x > rect->ur.x)
            rect->ur.x = x;
        if(x < rect->bl.x)
            rect->bl.x = x;
        if(y > rect->ur.y)
            rect->ur.y = y;
        if(y < rect->bl.y)
            rect->bl.y = y;
    }
}

void SimpleLayouter::PlaceOnBoard()
{
    int i;
    for(i = 0; i < (int)graphSet.size(); i++)
    {

    }
}
