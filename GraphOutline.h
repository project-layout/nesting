#ifndef GRAPHOUTLINE_H
#define GRAPHOUTLINE_H

#include <vector>

#include "GraphData.h"


class GraphOutline
{
public:
    GraphOutline();
    virtual ~GraphOutline();

    int AddPoint(double x, double y);
    int AddLine(LineType type, LineParamUnion param);

    int GetPointNum() const { return pointSet.size(); }
    int GetLineNum() const { return lineSet.size(); }

    Point *GetPoint(int idx) { return idx >= 0 && idx < (int)pointSet.size() ? &pointSet[idx] : NULL; }
    Line *GetLine(int idx) { return idx >= 0 && idx < (int)lineSet.size() ? &lineSet[idx] : NULL; }

    void Print();   // for test purpose

protected:
private:
    std::vector<Point> pointSet;
    std::vector<Line> lineSet;
};

#endif // GRAPHOUTLINE_H
