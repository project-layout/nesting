#include <stdlib.h>
#include <string.h>

#include "DxfReader.h"

DxfReader::DxfReader(const char filename[], std::vector<GraphInfo> &graphSet)
    : filename(filename)
    , graphSet(graphSet)
{
    //ctor
}

DxfReader::~DxfReader()
{
    //dtor
}

bool DxfReader::ReadData()
{
    std::vector<Line> lineSet;
    // ReadDxf(lineSet);
    // XXX: Currently, only consider one graph in a dxf file.
    ConstructGraph(lineSet);
    return false;
}

bool DxfReader::ConstructGraph(const std::vector<Line> &lineSet)
{
    unsigned int i;
    GraphInfo graphInfo;
    bool *used;
    unsigned int usedCount = 0;

    used = (bool *)malloc(sizeof(bool) * lineSet.size());
    memset(used, false, sizeof(bool) * lineSet.size());

    while(usedCount < lineSet.size()) // There are lines remain not used.
    {
        graphInfo.graph = new Graph;
        graphInfo.num = 1;  // XXX: Where to get the number?
        graphInfo.graph->SetName("");   // XXX: What's the name?

        // Find the first line that is not used.
        for(i = 0; i < lineSet.size(); i++)
            if(!used[i])
                break;

    }

    delete[] used;
    return true;
}

