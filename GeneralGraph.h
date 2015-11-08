#ifndef GENERALGRAPH_H
#define GENERALGRAPH_H

#include <Graph.h>


class GeneralGraph : public Graph
{
public:
    GeneralGraph();
    virtual ~GeneralGraph();

protected:
private:
    std::vector<GraphOutline *> outlineSet;
};

#endif // GENERALGRAPH_H
