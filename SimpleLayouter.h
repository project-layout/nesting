#ifndef SIMPLELAYOUTER_H
#define SIMPLELAYOUTER_H

#include <list>

#include "ILayouter.h"
#include "GeneralGraph.h"

class SimpleLayouter : public ILayouter
{
public:
    SimpleLayouter(const std::vector<GraphInfo> &graphSet, Point boardSize, std::vector<NestingResult> &result, double precision);
    virtual ~SimpleLayouter();

    bool Solve();

protected:
private:
    struct NestingUnit  // Used for storing nesting information
    {
        const GraphInfo *graphInfo;
        double rotate;
        Rect boundingBox;
    };

    struct LayoutColumn  // Column information used during nesting
    {
        double left;
        double right;
        double height;
    };

private:
    void BoundingBox(const GeneralGraph *graph, double rot, Rect *rect);
    void BoundingBoxForLine(const GeneralGraph::Line *line, double rot, Rect *rect);
    void BoundingBoxForArc(const GeneralGraph::Line *line, double rot, Rect *rect);
    void BoundingBoxForCircle(const GeneralGraph::Line *line, double rot, Rect *rect);
    bool CompareRectangleArea(const Rect &rect1, const Rect &rect2);

    void Nest();

    static bool CompareNestingUnit(const NestingUnit &u1, const NestingUnit &u2);

    void InitLayoutColumnList(std::list<LayoutColumn> &colList);

private:
    const std::vector<GraphInfo> &graphSet;
    Point boardSize;
    double precision;

    std::vector<NestingUnit> unitSet;
    std::vector<NestingResult> &result;
};

#endif // SIMPLELAYOUTER_H
