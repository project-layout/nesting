#include <math.h>
#include <sstream>

#include "ScrWriter.h"
#include "Graph.h"

#define POSTFIX ".scr"

ScrWriter::ScrWriter(const char outname[], std::vector<NestingResult> &result)
    : outname(outname)
    , result(result)
{
    fout.precision(3);
    fout.setf(std::ios::fixed);
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
                fout << "ZOOM ALL" << std::endl;
                fout.close();
            }
            curFileId++;
            ss << curFileId;
            ss >> strFileId;
            filename = outname + strFileId + POSTFIX;
            fout.open(filename);
            fout << "LIMITS\n" << (double)0 << "," << (double)0 << "\n" << page.x << "," << page.y << "\n\n";
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
                OutputScrArc(*line, result[i].pos, result[i].angle);
                break;
            case CIRCLE:
                break;
            default:
                break;
            }
        }
    }

    if(fout.is_open())
    {
        fout << "ZOOM ALL" << std::endl;
        fout.close();
    }
    return true;
}

void ScrWriter::OutputScrLine(const Line &line, const Point &offset, double rot)
{
    double x, y;
    fout << "LINE\n";
    x = line.param.lineParam.ep1.x + offset.x;
    y = line.param.lineParam.ep1.y + offset.y;
    fout << x << "," << y << "\n";
    x = line.param.lineParam.ep2.x + offset.x;
    y = line.param.lineParam.ep2.y + offset.y;
    fout << x << "," << y << "\n\n";
}


void ScrWriter::OutputScrArc(const Line &line, const Point &offset, double rot)
{
    double x, y;
    double radius = line.param.arcParam.radius;
    double startAng = line.param.arcParam.startAng;
    double endAng = line.param.arcParam.endAng;

    fout << "ARC\nC\n";
    x = line.param.arcParam.center.x + offset.x;
    y = line.param.arcParam.center.y + offset.y;
    fout << x << "," << y << "\n";
    x = line.param.arcParam.center.x + radius * cos(endAng) + offset.x;
    y = line.param.arcParam.center.y + radius * sin(endAng) + offset.y;
    fout << x << "," << y << "\n";
    x = line.param.arcParam.center.x + radius * cos(startAng) + offset.x;
    y = line.param.arcParam.center.y + radius * sin(startAng) + offset.y;
    fout << x << "," << y << "\n\n";
}
