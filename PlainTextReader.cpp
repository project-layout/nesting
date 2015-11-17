#include <stdio.h>
#include <string.h>
#include <limits>

#include "PlainTextReader.h"

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
    char input_line[INPUT_LINE_SIZE]; // Line will be truncated if its length >= INPUT_LINE_SIZE.
    char name[INPUT_LINE_SIZE];
    char typeStr[INPUT_LINE_SIZE];
    GraphInfo graphInfo;
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

    graphInfo.graph = new Graph;
    graphInfo.graph->SetName(name);

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

        Line line;
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
            case LINE:
                line.param.lineParam.ep1 = pointSet[i];
                line.param.lineParam.ep2 = pointSet[(i+1)%pointNum];
                break;
            case ARC:
                line.param.arcParam.ep1 = pointSet[i];
                line.param.arcParam.ep2 = pointSet[(i+1)%pointNum];
                if(sscanf(input_line, "%*s%lf%d", &line.param.arcParam.radian, &line.param.arcParam.zDir) < 2)
                    continue;
                break;
            case CIRCLE:
                line.param.circleParam.center = pointSet[i];
                if(sscanf(input_line, "%*s%lf", &line.param.circleParam.radius) < 1)
                    continue;
                break;
            default:
                break;
            }
            graphInfo.graph->AddLine(line);
            ++i;
        }
    }

    if(suc)
    {
        graphSet.push_back(graphInfo);
    }
    else
    {
        delete graphInfo.graph;
    }

    return suc;
}

LineType PlainTextReader::GetLineType(char typeStr[])
{
    if(strcmp(typeStr, "LINE") == 0)
        return LINE;
    if(strcmp(typeStr, "ARC") == 0)
        return ARC;
    if(strcmp(typeStr, "CIRCLE") == 0)
        return CIRCLE;
    return LINE;    // default is straight line
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
