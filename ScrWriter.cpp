#include <math.h>
#include <sstream>

#include "ScrWriter.h"
#include "Graph.h"

#define POSTFIX ".scr"

ScrWriter::ScrWriter(const char outname[], std::vector<NestingResult> &result)
    : outname(outname)
    , result(result)
{
}

ScrWriter::~ScrWriter()
{
}

bool ScrWriter::OutputFile()
{
    int i, j, curFileId = -1;
    std::string strFileId, filename;
    std::stringstream ss;
    const Graph *graph;

    for(i = 0; i < (int)result.size(); i++)
    {
        if(result[i].board != curFileId)
        {
            if(fout.is_open())
            {
                fout << "ZOOM E" << std::endl;
                fout.close();
            }
            curFileId++;
            ss << curFileId;
            ss >> strFileId;
            filename = outname + strFileId + POSTFIX;
            fout.open(filename);
        }

        graph = result[i].graph;
        for(j = 0; j < graph->GetLineNum(); j++)
        {
            const Line *line = graph->GetLine(j);
            switch(line->type)
            {
            case LINE:
                OutputScrLine(*line, result[i].pos, result[i].angle);
                break;
            case ARC:
                break;
            case CIRCLE:
                break;
            default:
                break;
            }
        }
    }

    if(fout.is_open())
        fout.close();
    return true;
}

void ScrWriter::OutputScrLine(const Line &line, const Point &offset, double angle)
{
    double x, y;
    fout << "LINE ";
    x = line.param.lineParam.ep1.x + offset.x;
    y = line.param.lineParam.ep1.y + offset.y;
    fout << x << " ," << y << " ,0.000000 ,";
    x = line.param.lineParam.ep2.x + offset.x;
    y = line.param.lineParam.ep2.y + offset.y;
    fout << x << " ," << y << " ,0.000000 " << std::endl;
}


void ScrWriter::OutputScrArc(const Line &line, const Point &offset, double angle)
{
    double x, y;
    double radius = line.param.arcParam.radius;
    double startAng = line.param.arcParam.startAng;
    double endAng = line.param.arcParam.endAng;

    fout << "ARC C ";
    x = line.param.arcParam.center.x + offset.x;
    y = line.param.arcParam.center.y + offset.y;
    fout << x << " ," << y << " ,0.000000 ,";
    x = line.param.arcParam.center.x + radius * cos(startAng) + offset.x;
    y = line.param.arcParam.center.y + radius * sin(startAng) + offset.y;
    fout << x << " ," << y << " A " << (endAng - startAng) << std::endl;
}
