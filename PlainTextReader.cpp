#include <stdio.h>
#include <string.h>
#include <limits>
#include <math.h>

#include "PlainTextReader.h"
#include "Utils.h"

#define INPUT_LINE_SIZE 1024

PlainTextReader::PlainTextReader(const char filename[], std::vector<GraphInfo> &graphSet)
    : filename(filename)
    , graphSet(graphSet)
{
}

PlainTextReader::~PlainTextReader()
{
    //dtor
}

bool PlainTextReader::ReadData()
{
    fin.open(filename.c_str());
    if(!fin.is_open())
    {
        printf("Cannot open the file: %s\n", filename.c_str());
        return false;
    }

    bool suc = true;
    while(!fin.eof() && suc)
    {
        suc = ReadGraph();
    }

    if(!suc)
    {
        printf("Failed to read graph from the file.\n");
        for(int i = 0; i < (int)graphSet.size(); i++)
            delete graphSet[i].graph;
        graphSet.clear();
    }

    fin.close();
    return suc;
}

bool PlainTextReader::ReadGraph()
{
    int i;
    bool suc;
    char input_line[INPUT_LINE_SIZE]; // Input line will be truncated if its length >= INPUT_LINE_SIZE.
    char name[INPUT_LINE_SIZE];
    char typeStr[INPUT_LINE_SIZE];
    GraphInfo graphInfo;
    GeneralGraph *graph;
    Point point;
    std::vector<Point> pointSet;

    while(!fin.eof())
    {
        ReadInputLine(input_line, INPUT_LINE_SIZE);
        if(!IgnoredLine(input_line, INPUT_LINE_SIZE))
        {
            if(sscanf(input_line, "%s%d", name, &graphInfo.num) < 2)
                return false;
            break;
        }
    }

    graph = new GeneralGraph;
    graph->SetName(name);

    suc = false;
    while(!fin.eof())   // For each graph outline
    {
        int pointNum = 0;
        while(!fin.eof())   // Read the number of points
        {
            ReadInputLine(input_line, INPUT_LINE_SIZE);
            if(strcmp(input_line, "END") == 0)
            {
                suc = true;
                break;
            }
            if(sscanf(input_line, "%d", &pointNum) >= 1)
                break;
        }
        if(suc)
            break;

        i = 0;
        pointSet.clear();
        while(!fin.eof() && i < pointNum)   // Read each point
        {
            ReadInputLine(input_line, INPUT_LINE_SIZE);
            if(sscanf(input_line, "%lf%lf", &point.x, &point.y) < 2)
                continue;
            pointSet.push_back(point);
            ++i;
        }

        GeneralGraph::Line line;
        i = 0;
        while(!fin.eof() && i < pointNum)   // Read each line
        {
            ReadInputLine(input_line, INPUT_LINE_SIZE);
            if(IgnoredLine(input_line, INPUT_LINE_SIZE))
                continue;
            if(sscanf(input_line, "%s", typeStr) < 1)
                continue;
            line.type = GetLineType(typeStr);
            switch(line.type)
            {
            case GeneralGraph::LINE:
                line.param.lineParam.ep1 = pointSet[i];
                line.param.lineParam.ep2 = pointSet[(i+1)%pointNum];
                break;
            case GeneralGraph::ARC:
                {
                    double radian;
                    int zDir;
                    if(sscanf(input_line, "%*s%lf%d", &radian, &zDir) < 2)
                        continue;
                    line.param.arcParam.center = GetCircleCenter(pointSet[i], pointSet[(i+1)%pointNum], radian, zDir);

                    const Point &p1 = pointSet[i];
                    const Point &p2 = pointSet[(i+1)%pointNum];
                    const Point &pc = line.param.arcParam.center;
                    double x1 = p1.x, x2 = p2.x, y1 = p1.y, y2 = p2.y;
                    double chordLen = sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
                    line.param.arcParam.radius = chordLen/2/sin(radian/2);
                    line.param.arcParam.startAng = atan2(p1.y-pc.y, p1.x-pc.x);
                    line.param.arcParam.endAng = atan2(p2.y-pc.y, p2.x-pc.x);
                }
                break;
            case GeneralGraph::CIRCLE:
                line.param.circleParam.center = pointSet[i];
                if(sscanf(input_line, "%*s%lf", &line.param.circleParam.radius) < 1)
                    continue;
                break;
            default:
                break;
            }
            graph->AddLine(line);
            ++i;
        }
    }

    if(suc)
    {
        graphInfo.graph = graph;
        graphSet.push_back(graphInfo);
    }
    else
    {
        delete graphInfo.graph;
    }

    return suc;
}

GeneralGraph::LineType PlainTextReader::GetLineType(char typeStr[])
{
    if(strcmp(typeStr, "LINE") == 0)
        return GeneralGraph::LINE;
    if(strcmp(typeStr, "ARC") == 0)
        return GeneralGraph::ARC;
    if(strcmp(typeStr, "CIRCLE") == 0)
        return GeneralGraph::CIRCLE;
    return GeneralGraph::LINE;    // default is straight line
}

void PlainTextReader::ReadInputLine(char line[], int len)
{
    fin.getline(line, len);
    if(fin.fail())
    {
        if(!fin.eof())
            fin.clear();
        fin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

inline bool PlainTextReader::IgnoredLine(char line[], int len)
{
    return len <= 0 || line[0] == '\0' || line[0] == '\n' || line[0] == '\r' || line[0] == '#';
}
