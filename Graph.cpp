#include <stdio.h>

#include "Graph.h"

Graph::Graph()
{
    //ctor
}

Graph::~Graph()
{
    for(int i = 0; i < (int)outlineSet.size(); i++)
        if(outlineSet[i])
            delete outlineSet[i];
}

int Graph::AddOutline(GraphOutline *outline)
{
    outlineSet.push_back(outline);
    return outlineSet.size()-1;
}

void Graph::Print()
{
    printf("Graph name: %s\n", name.c_str());
    printf("The number of outlines: %d\n", outlineSet.size());
    for(int i = 0; i < (int)outlineSet.size(); i++)
    {
        printf("Outline %d\n", i);
        outlineSet[i]->Print();
    }
}
