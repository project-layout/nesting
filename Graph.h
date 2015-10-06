#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <string>

#include "GraphOutline.h"


class Graph
{
public:
    Graph();
    ~Graph();

    int AddOutline(GraphOutline *outline);
    void SetName(const char name[]) { this->name = name; }
    int GetOutlineNum() const { return outlineSet.size(); }
    GraphOutline *GetOutline(int idx) const { if(idx >= 0 && idx < (int)outlineSet.size()) return outlineSet[idx]; else return NULL; }

    void Print();   // for test purpose

protected:
private:
    int id;
    std::string name;
    std::vector<GraphOutline *> outlineSet;
};

#endif // GRAPH_H
