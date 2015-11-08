#include <stdio.h>

#include "Graph.h"
#include "Utils.h"

Graph::Graph()
{
    //ctor
}

Graph::~Graph()
{
}

int Graph::AddLine(const Line &line)
{
    lineSet.push_back(line);
    return lineSet.size()-1;
}

void Graph::Print()
{
    printf("Graph name: %s\n", name.c_str());
    printf("The number of lines: %d\n", lineSet.size());
    for(int i = 0; i < (int)lineSet.size(); i++)
    {
        printf("Line %d: %s\n", i, GetLineNameStr(lineSet[i]));
        switch(lineSet[i].type)
        {
        case LINE:
            printf("  (%.4lf,%.4lf)->(%.4lf,%.4lf)\n",
                   lineSet[i].param.lineParam.ep1.x,
                   lineSet[i].param.lineParam.ep1.y,
                   lineSet[i].param.lineParam.ep2.x,
                   lineSet[i].param.lineParam.ep2.y);
            break;
        case ARC:
            printf("  (%.4lf,%.4lf)->(%.4lf,%.4lf)",
                   lineSet[i].param.arcParam.ep1.x,
                   lineSet[i].param.arcParam.ep1.y,
                   lineSet[i].param.arcParam.ep2.x,
                   lineSet[i].param.arcParam.ep2.y);
            printf(" radian=%lf dir=%d\n", lineSet[i].param.arcParam.radian, lineSet[i].param.arcParam.zDir);
            break;
        case CIRCLE:
            printf("  center: (%.4lf, %.4lf), radius: %.4lf\n",
                   lineSet[i].param.circleParam.center.x,
                   lineSet[i].param.circleParam.center.y,
                   lineSet[i].param.circleParam.radius);
            break;
        default:
            printf("\n");
        }
    }
}
