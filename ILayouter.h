#ifndef ILAYOUTER_H
#define ILAYOUTER_H

#include <vector>

#include "GraphData.h"


struct NestingResult
{
    int board;
    Point pos;
    double angle;
};

class ILayouter
{
public:
    ILayouter();
    virtual ~ILayouter();

    virtual bool Solve() = 0;

protected:
private:
};

#endif // ILAYOUTER_H
