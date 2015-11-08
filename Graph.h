#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <string>

#include "GraphData.h"

class Graph
{
public:
    Graph();
    ~Graph();

    int AddLine(const Line &line);
    void SetName(const char name[]) { this->name = name; }
    int GetLineNum() const { return lineSet.size(); }
    const Line *GetLine(int idx) const { if(idx >= 0 && idx < (int)lineSet.size()) return &lineSet[idx]; else return NULL; }

    void Print();   // for test purpose

protected:
private:
    std::string name;
    std::vector<Line> lineSet;
};

#endif // GRAPH_H
