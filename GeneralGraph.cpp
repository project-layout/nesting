#include <stdio.h>
#include <math.h>

#include "GeneralGraph.h"
#include "Utils.h"

GeneralGraph::GeneralGraph()
{
    //ctor
}

GeneralGraph::~GeneralGraph()
{
    //dtor
}

int GeneralGraph::AddLine(const Line &line)
{
    lineSet.push_back(line);
    return (int)lineSet.size()-1;
}

void GeneralGraph::Print()
{
    printf("Graph name: %s\n", name.c_str());
    printf("The number of lines: %d\n", (int)lineSet.size());
    for(int i = 0; i < (int)lineSet.size(); i++)
    {
        printf("Line %d: %s\n", i, GetLineNameStr(lineSet[i]));
        switch(lineSet[i].type)
        {
            case LINE:
                printf("  (%.4f,%.4f)->(%.4f,%.4f)\n",
                       lineSet[i].param.lineParam.ep1.x,
                       lineSet[i].param.lineParam.ep1.y,
                       lineSet[i].param.lineParam.ep2.x,
                       lineSet[i].param.lineParam.ep2.y);
                break;
            case ARC:
                printf("  (%.4f,%.4f)->(%.4f,%.4f)",
                       lineSet[i].param.arcParam.center.x+lineSet[i].param.arcParam.radius*cos(lineSet[i].param.arcParam.startAng),
                       lineSet[i].param.arcParam.center.y+lineSet[i].param.arcParam.radius*sin(lineSet[i].param.arcParam.startAng),
                       lineSet[i].param.arcParam.center.x+lineSet[i].param.arcParam.radius*cos(lineSet[i].param.arcParam.endAng),
                       lineSet[i].param.arcParam.center.y+lineSet[i].param.arcParam.radius*sin(lineSet[i].param.arcParam.endAng));
                printf(" radius=%f center=(%f,%f) start_angle=%f end_angle=%f\n",
                       lineSet[i].param.arcParam.radius,
                       lineSet[i].param.arcParam.center.x,
                       lineSet[i].param.arcParam.center.y,
                       lineSet[i].param.arcParam.startAng,
                       lineSet[i].param.arcParam.endAng);
                break;
            case CIRCLE:
                printf("  center: (%.4f, %.4f), radius: %.4f\n",
                       lineSet[i].param.circleParam.center.x,
                       lineSet[i].param.circleParam.center.y,
                       lineSet[i].param.circleParam.radius);
                break;
            default:
                printf("\n");
        }
    }
}
