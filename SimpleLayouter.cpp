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
    double rot;
    Rect rect;

    // Compute the bounding boxes
    unitSet.resize(graphSet.size());
    for(i = 0; i < (int)graphSet.size(); i++)
    {
        if(graphSet[i].graph->GetType() == Graph::GENERAL)
        {
            unitSet[i].graphInfo = &graphSet[i];
            unitSet[i].boundingBox.bl.x = 0;    // Initial max bounding box, larger than the board
            unitSet[i].boundingBox.bl.y = 0;
            unitSet[i].boundingBox.ur.x = boardSize.x + 1;
            unitSet[i].boundingBox.ur.y = boardSize.y + 1;

            for(rot = 0; FloatLess(rot, 2*PI, EPS); rot += precision)
            {
                BoundingBox((GeneralGraph *)graphSet[i].graph, rot, &rect);
                if(CompareRectangleArea(rect, unitSet[i].boundingBox))
                {
                    unitSet[i].boundingBox = rect;
                    unitSet[i].rotate = rot;
                }
            }

            if(unitSet[i].boundingBox.ur.y - unitSet[i].boundingBox.bl.y > boardSize.y
               || unitSet[i].boundingBox.ur.x - unitSet[i].boundingBox.bl.x > boardSize.x)
            {
                // This graph cannot be nested into the board.
                return false;
            }
        }
        else
        {
            // TODO: Need to add supports for other type of graphs
        }
    }

    // Nest the graphs by the bounding boxes
    Nest();

    return true;
}

void SimpleLayouter::BoundingBox(const GeneralGraph *graph, double rot, Rect *rect)
{
    int i;
    Rect lineRect;
    rect->ur.x = rect->ur.y = -(rect->bl.x = rect->bl.y = DBL_MAX);
    for(i = 0; i < graph->GetLineNum(); i++)
    {
        const GeneralGraph::Line *line = graph->GetLine(i);
        switch(line->type)
        {
        case GeneralGraph::LINE:
            BoundingBoxForLine(line, rot, &lineRect);
            break;
        case GeneralGraph::ARC:
            BoundingBoxForArc(line, rot, &lineRect);
            break;
        case GeneralGraph::CIRCLE:
            BoundingBoxForCircle(line, rot, &lineRect);
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

void SimpleLayouter::BoundingBoxForLine(const GeneralGraph::Line *line, double rot, Rect *rect)
{
    const Point &p1 = line->param.lineParam.ep1;
    const Point &p2 = line->param.lineParam.ep2;
    double x1 = p1.x, x2 = p2.x, y1 = p1.y, y2 = p2.y;
    double x1r = x1*cos(rot)-y1*sin(rot);
    double y1r = y1*cos(rot)+x1*sin(rot);
    double x2r = x2*cos(rot)-y2*sin(rot);
    double y2r = y2*cos(rot)+x2*sin(rot);

    rect->bl.x = x1r < x2r ? x1r : x2r;
    rect->bl.y = y1r < y2r ? y1r : y2r;
    rect->ur.x = x1r > x2r ? x1r : x2r;
    rect->ur.y = y1r > y2r ? y1r : y2r;
}

void SimpleLayouter::BoundingBoxForArc(const GeneralGraph::Line *line, double rot, Rect *rect)
{
    const Point &center = line->param.arcParam.center;
    double cxr = center.x*cos(rot)-center.y*sin(rot);
    double cyr = center.y*cos(rot)+center.x*sin(rot);
    double radius = line->param.arcParam.radius;
    double startAng = line->param.arcParam.startAng+rot;
    double endAng = line->param.arcParam.endAng+rot;
    double angle, x, y;

    rect->ur.x = rect->ur.y = -(rect->bl.x = rect->bl.y = DBL_MAX);
    for(angle = startAng; angle < endAng+EPS; angle += precision)
    {
        x = cxr+radius*cos(angle);
        y = cyr+radius*sin(angle);
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

void SimpleLayouter::BoundingBoxForCircle(const GeneralGraph::Line *line, double rot, Rect *rect)
{
    const Point &center = line->param.circleParam.center;
    double cxr = center.x*cos(rot)-center.y*sin(rot);
    double cyr = center.y*cos(rot)+center.x*sin(rot);

    rect->ur.x = cxr + line->param.circleParam.radius;
    rect->ur.y = cyr + line->param.circleParam.radius;
    rect->bl.x = cxr - line->param.circleParam.radius;
    rect->bl.y = cyr - line->param.circleParam.radius;
}

bool SimpleLayouter::CompareRectangleArea(const Rect &rect1, const Rect &rect2)
{
    double area1 = (rect1.ur.x - rect1.bl.x) * (rect1.ur.y - rect1.bl.y);
    double area2 = (rect2.ur.x - rect2.bl.x) * (rect2.ur.y - rect2.bl.y);
    return area1 < area2;
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
        Rect &rect = unitSet[i].boundingBox;
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
                while(FloatGreater(curY + height, boardSize.y - it->height, EPS))    // Find a position high enough
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
            nr.graph = unitSet[i].graphInfo->graph;
            nr.board = curBoard;
            nr.pos.x = curX - rect.bl.x;
            nr.pos.y = curY - rect.bl.y;
            nr.angle = unitSet[i].rotate;
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
                it2 = it = colList.insert(it, col);
                ++it2;
                if(colList.end() != it2)
                {
                    it2->left = it->right;
                }
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
    const Rect &r1 = u1.boundingBox, &r2 = u2.boundingBox;
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
