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
    char line[INPUT_LINE_SIZE]; // Line will be truncated if its length >= INPUT_LINE_SIZE.
    char name[INPUT_LINE_SIZE];
    char typeStr[INPUT_LINE_SIZE];
    GraphInfo graphInfo;

    while(!fin.eof())
    {
        ReadInputLine(line);
        if(!IgnoredLine(line, INPUT_LINE_SIZE))
        {
            if(sscanf(line, "%s%d", name, &graphInfo.num) < 2)
                return false;
            break;
        }
    }

    graphInfo.graph = new Graph;
    graphInfo.graph->SetName(name);
    GraphOutline *outline;

    suc = false;
    while(!fin.eof())   // For each graph outline
    {
        int pointNum = 0;
        while(!fin.eof())   // Read the number of points
        {
            ReadInputLine(line);
            if(strcmp(line, "END") == 0)
            {
                suc = true;
                break;
            }
            if(sscanf(line, "%d", &pointNum) >= 1)
                break;
        }
        if(suc)
            break;

        outline = new GraphOutline;

        i = 0;
        double x, y;
        while(!fin.eof() && i < pointNum)   // Read each point
        {
            ReadInputLine(line);
            if(sscanf(line, "%lf%lf", &x, &y) < 2)
                continue;
            outline->AddPoint(x, y);
            ++i;
        }

        LineType type;
        LineParamUnion param;
        i = 0;
        while(!fin.eof() && i < pointNum)   // Read each line
        {
            ReadInputLine(line);
            if(IgnoredLine(line, INPUT_LINE_SIZE))
                continue;
            if(sscanf(line, "%s", typeStr) < 1)
                continue;
            type = GetLineType(typeStr);
            switch(type)
            {
            case LINE:
                memset(&param, 0, sizeof(param));
                break;
            case ARC:
                int zDir;
                if(sscanf(line, "%*s%lf%d", &param.arcParam.radian, &zDir) < 2)
                    continue;
                param.arcParam.zDir = zDir;
                break;
            default:
                break;
            }
            outline->AddLine(type, param);
            ++i;
        }

        if(i == pointNum)
        {
            graphInfo.graph->AddOutline(outline);
        }
        else
        {
            delete outline;
            break;
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
    return LINE;    // default is straight line
}

void PlainTextReader::ReadInputLine(char line[])
{
    fin.getline(line, INPUT_LINE_SIZE);
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
