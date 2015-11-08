#include <stdio.h>
#include <math.h>
#include <float.h>
#include <algorithm>
#include <list>

#include "SimpleLayouter.h"
#include "Graph.h"
#include "Utils.h"
#include "CommonConstants.h"


SimpleLayouter::SimpleLayouter(const std::vector<GraphInfo> &graphSet, Point boardSize, std::vector<NestingResult> &result, double precision)
    : graphSet(graphSet)
    , boardSize(boardSize)
    , precision(precision)
    , result(result)
{
    unitSet.reserve(graphSet.size());
}

SimpleLayouter::~SimpleLayouter()
{
    //dtor
}

bool SimpleLayouter::Solve()
{
    int i;
    Rect rect;
    unitSet.resize(graphSet.size());
    for(i = 0; i < (int)graphSet.size(); i++)
    {
        EnclosureRect(graphSet[i].graph, &rect);
        unitSet[i].idx = i;
        unitSet[i].graphInfo = &graphSet[i];
        unitSet[i].enclosureRect = rect;
        if(rect.ur.y-rect.bl.y > boardSize.y || rect.ur.x-rect.bl.x > boardSize.x)  // This graph cannot be nested into the board.
            return false;
    }

    Nest();
    return true;
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
            case CIRCLE:
                EnclosureRectForCircle(line, point1, &lineRect);
                break;
            default:
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

void SimpleLayouter::EnclosureRectForCircle(const Line *line, const Point *center, Rect *rect)
{
    rect->ur.x = center->x + line->param.circleParam.radius;
    rect->ur.y = center->y + line->param.circleParam.radius;
    rect->bl.x = center->x - line->param.circleParam.radius;
    rect->bl.y = center->y - line->param.circleParam.radius;
}

void SimpleLayouter::Nest()
{
    int i, j;
    int curBoard;
    bool wide, nextBoard;
    double curX, curY, width, height;
    LayoutColumn col;
    std::list<LayoutColumn> colList;
    std::list<LayoutColumn>::iterator it, it2, firstIt;
    NestingResult nr;

    // The initial state of the board, no graph placed
    InitLayoutColumnList(colList);

    // Sort all types of graphs
    std::sort(unitSet.begin(), unitSet.end(), CompareNestingUnit);

    // Place all graphs onto boards
    curBoard = 0;
    it = colList.begin();
    curX = curY = 0;
    for(i = 0; i < (int)unitSet.size(); i++)
    {
        Rect &rect = unitSet[i].enclosureRect;
        width = rect.ur.x - rect.bl.x;
        height = rect.ur.y - rect.bl.y;
        for(j = 0; j < unitSet[i].graphInfo->num; j++)
        {
            firstIt = it;
            // Find a position to place the current enclosure rectangle.
            while(true)
            {
                // Check if the current rectangle can be placed on the current board.
                nextBoard = false;
                while(FloatGreater(curY + height, it->height, EPS))    // Find a position high enough
                {
                    ++it;
                    curX = it->left;
                    curY = it->height;
                    if(colList.end() == it)
                        it = colList.begin();
                    if(it == firstIt)
                    {
                        nextBoard = true;
                        break;
                    }
                }
                if(nextBoard)
                {
                    ++curBoard;
                    InitLayoutColumnList(colList);
                    firstIt = it = colList.begin();
                    curX = curY = 0;
                    continue;
                }

                wide = true;
                it2 = it;
                while(colList.end() != it2 && FloatGreater(curX + width, it2->right, EPS))  // Check if this position is wide enough
                {
                    ++it2;
                    if(colList.end() == it2 || FloatGreater(it2->height, curY, EPS))
                    {
                        wide = false;
                        break;
                    }
                }
                if(!wide)
                {
                    it = colList.end() == it2 ? colList.begin() : it2;
                    curX = it->left;
                    curY = it->height;
                    continue;
                }

                break;
            }

            // Place it at the current position.
            nr.board = curBoard;
            nr.pos.x = curX - rect.bl.x;
            nr.pos.y = curY - rect.bl.y;
            nr.angle = 0;
            result.push_back(nr);

            // Modify the linked list
            while(FloatGreater(curX + width, it->right, EPS))
            {
                it = colList.erase(it);
            }
            if(FloatLess(curX + width, it->right, EPS)) // shorter, insert a node
            {
                col.left = curX;
                col.right = curX + width;
                col.height = curY + height;
                it = colList.insert(it, col);
            }
            else
            {
                it->left = curX;
                it->height = curY + height;
            }

            // Advance the iterator to the next one.
            ++it;
            if(colList.end() == it)
                it = colList.begin();
            curX = it->left;
            curY = it->height;
        }
    }
}

bool SimpleLayouter::CompareNestingUnit(const NestingUnit &u1, const NestingUnit &u2)   // compared by height first, then width
{
    const Rect &r1 = u1.enclosureRect, &r2 = u2.enclosureRect;
    double w1 = r1.ur.x-r1.bl.x;
    double w2 = r2.ur.x-r2.bl.x;
    double h1 = r1.ur.y-r1.bl.y;
    double h2 = r2.ur.y-r2.bl.y;
    if(h1 > h2)
        return true;
    if(h1 < h2)
        return false;
    if(w1 > w2)
        return true;
    return false;
}

void SimpleLayouter::InitLayoutColumnList(std::list<LayoutColumn> &colList)
{
    LayoutColumn col;
    colList.clear();
    col.left = 0;
    col.right = boardSize.x;
    col.height = 0;
    colList.push_back(col);
}
