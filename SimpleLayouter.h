#ifndef SIMPLELAYOUTER_H
#define SIMPLELAYOUTER_H

#include "ILayouter.h"


class SimpleLayouter : public ILayouter
{
public:
    SimpleLayouter(const std::vector<GraphInfo> &graphSet, Point boardSize, std::vector<LayoutResult> &result, double precision);
    virtual ~SimpleLayouter();

    void Solve();

protected:
private:
    void EnclosureRect(const Graph *graph, Rect *rect);
    void EnclosureRectForLine(const Line *line, const Point *p1, const Point *p2, Rect *rect);
    void EnclosureRectForArc(const Line *line, const Point *p1, const Point *p2, Rect *rect);

    void PlaceOnBoard();

private:
    static const double EPS = 1e-10;
    const std::vector<GraphInfo> &graphSet;
    Point boardSize;
    double precision;

    std::vector<Rect> enclosureRect;
    std::vector<LayoutResult> &result;
};

#endif // SIMPLELAYOUTER_H
