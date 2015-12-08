#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <string>

#include "GraphData.h"

// Base class for all kinds of graphs
class Graph
{
public:
    enum Type
    {
        GENERAL,
        RECT,
        CIRCLE,
        TRIANGLE,
        ELLIPSE,
        TRAPEZOID
    };

public:
    Graph();
    ~Graph();

    void SetName(const char name[]) { this->name = name; }
    std::string GetName() const { return name; }

    void SetType(Type type) { this->type = type; }
    Type GetType() const { return this->type; }

protected:
    Type type;
    std::string name;

private:

};

#endif // GRAPH_H
