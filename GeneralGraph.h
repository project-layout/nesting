#ifndef GENERALGRAPH_H
#define GENERALGRAPH_H

#include <string>
#include <vector>

#include "Graph.h"

class GeneralGraph : public Graph
{
public:
    enum LineType
    {
        LINE,
        ARC,
        CIRCLE
    };

    struct LineParam
    {
        Point ep1, ep2;
    };

    struct ArcParam
    {
        Point center;
        double radius;
        double startAng, endAng;
    };

    struct CircleParam
    {
        Point center;
        double radius;
    };

    union LineParamUnion
    {
        LineParam lineParam;
        ArcParam arcParam;
        CircleParam circleParam;
    };

    struct Line
    {
        LineType type;
        LineParamUnion param;
    };

public:
    GeneralGraph();
    virtual ~GeneralGraph();

    int AddLine(const Line &line);
    int GetLineNum() const { return (int)lineSet.size(); }
    const Line *GetLine(int idx) const { if(idx >= 0 && idx < (int)lineSet.size()) return &lineSet[idx]; else return NULL; }
    
    void Print();   // for test purpose
    
protected:
private:
    std::vector<Line> lineSet;
};

#endif // GENERALGRAPH_H
