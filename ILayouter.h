#ifndef ILAYOUTER_H
#define ILAYOUTER_H

#include <vector>

#include "GraphData.h"


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
