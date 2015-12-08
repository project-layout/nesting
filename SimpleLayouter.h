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
    void EnclosureRect(const GeneralGraph *graph, Rect *rect);
    void EnclosureRectForLine(const GeneralGraph::Line *line, Rect *rect);
    void EnclosureRectForArc(const GeneralGraph::Line *line, Rect *rect);
    void EnclosureRectForCircle(const GeneralGraph::Line *line, Rect *rect);

    void Nest();

    struct NestingUnit  // Used for storing nesting information
    {
        const GraphInfo *graphInfo;
        Rect enclosureRect;
    };

    static bool CompareNestingUnit(const NestingUnit &u1, const NestingUnit &u2);

    struct LayoutColumn  // Column information used during nesting
    {
        double left;
        double right;
        double height;
    };

    void InitLayoutColumnList(std::list<LayoutColumn> &colList);

private:
    const std::vector<GraphInfo> &graphSet;
    Point boardSize;
    double precision;

    std::vector<NestingUnit> unitSet;
    std::vector<NestingResult> &result;
};

#endif // SIMPLELAYOUTER_H
