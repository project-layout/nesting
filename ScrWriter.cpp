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
    const GeneralGraph *graph;

    for(i = 0; i < (int)result.size(); i++)
    {
        if(result[i].board != curFileId)
        {
            if(fout.is_open())
            {
                fout << "ZOOM ALL\r\n" << std::endl;
                fout.close();
            }
            curFileId++;
            ss << curFileId;
            ss >> strFileId;
            filename = outname + strFileId + POSTFIX;
            fout.open(filename);
            fout << "LIMITS\r\n" << (double)0 << "," << (double)0 << "\r\n" << page.x << "," << page.y << "\r\n\r\n";
        }

        if(result[i].graph->GetType() == Graph::GENERAL)
        {
            graph = (GeneralGraph *)result[i].graph;
            for(j = 0; j < graph->GetLineNum(); j++)
            {
                const GeneralGraph::Line *line = graph->GetLine(j);
                switch(line->type)
                {
                    case GeneralGraph::LINE:
                        OutputScrLine(*line, result[i].pos, result[i].angle);
                        break;
                    case GeneralGraph::ARC:
                        OutputScrArc(*line, result[i].pos, result[i].angle);
                        break;
                    case GeneralGraph::CIRCLE:
                        break;
                    default:
                        break;
                }
            }
        }
    }

    if(fout.is_open())
    {
        fout << "ZOOM ALL\r\n" << std::endl;
        fout.close();
    }
    return true;
}

void ScrWriter::OutputScrLine(const GeneralGraph::Line &line, const Point &offset, double rot)
{
    double x, y, xr, yr;

    fout << "LINE\r\n";
    x = line.param.lineParam.ep1.x;
    y = line.param.lineParam.ep1.y;
    xr = x * cos(rot) - y * sin(rot);
    yr = y * cos(rot) + x * sin(rot);
    fout << xr + offset.x << "," << yr + offset.y << "\r\n";
    x = line.param.lineParam.ep2.x;
    y = line.param.lineParam.ep2.y;
    xr = x * cos(rot) - y * sin(rot);
    yr = y * cos(rot) + x * sin(rot);
    fout << xr + offset.x << "," << yr + offset.y << "\r\n\r\n";
}


void ScrWriter::OutputScrArc(const GeneralGraph::Line &line, const Point &offset, double rot)
{
    double x, y, xc, yc, xcr, ycr;
    double radius = line.param.arcParam.radius;
    double startAng = line.param.arcParam.startAng;
    double endAng = line.param.arcParam.endAng;

    fout << "ARC\r\nC\r\n";
    xc = line.param.arcParam.center.x;
    yc = line.param.arcParam.center.y;
    xcr = xc * cos(rot) - yc * sin(rot);
    ycr = yc * cos(rot) + xc * sin(rot);
    fout << xcr + offset.x << "," << ycr + offset.y << "\r\n";
    x = xcr + radius * cos(startAng + rot) + offset.x;
    y = ycr + radius * sin(startAng + rot) + offset.y;
    fout << x << "," << y << "\r\n";
    x = xcr + radius * cos(endAng + rot) + offset.x;
    y = ycr + radius * sin(endAng + rot) + offset.y;
    fout << x << "," << y << "\r\n";
}
