#ifndef SIMPLELAYOUTER_H
#define SIMPLELAYOUTER_H

#include "ILayouter.h"


class SimpleLayouter : public ILayouter
{
public:
    SimpleLayouter(const std::vector<GraphInfo> &graphSet, Point boardSize, std::vector<NestingResult> &result, double precision);
    virtual ~SimpleLayouter();

    bool Solve();

protected:
private:
    void EnclosureRect(const Graph *graph, Rect *rect);
    void EnclosureRectForLine(const Line *line, const Point *p1, const Point *p2, Rect *rect);
    void EnclosureRectForArc(const Line *line, const Point *p1, const Point *p2, Rect *rect);
    void EnclosureRectForCircle(const Line *line, const Point *center, Rect *rect);

    void Nest();

    struct NestingUnit  // Used for storing nesting information
    {
        int idx;
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
