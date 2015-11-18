#include <stdlib.h>
#include <string.h>

#include "DxfReader.h"
#include "Utils.h"

#define INPUT_LINE_SIZE 512

#define DXF_LINE "LINE"
#define DXF_ARC "ARC"
#define DXF_CIRCLE "CIRCLE"
#define DXF_EOF "EOF"

#define DXF_ENTITY_TYPE 0
#define DXF_COORD_X0 10
#define DXF_COORD_Y0 20
#define DXF_COORD_Z0 30
#define DXF_COORD_X1 11
#define DXF_COORD_Y1 21
#define DXF_COORD_Z1 31
#define DXF_RADIUS 40
#define DXF_START_ANGLE 50
#define DXF_END_ANGLE 51

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
    // XXX: Currently, only consider one graph in a dxf file.
    Graph *graph;
    char code[INPUT_LINE_SIZE];
    char value[INPUT_LINE_SIZE];

    fin.open(filename.c_str());
    if(!fin.is_open())
    {
        printf("Cannot open the file: %s\n", filename.c_str());
        return false;
    }

    graph = new Graph;
    graph->SetName("DXF graph");    // TODO: name?

    fin.getline(code, INPUT_LINE_SIZE);
    fin.getline(value, INPUT_LINE_SIZE);
    Trim(value);
    while(!fin.eof())
    {
        if(strcmp(value, DXF_LINE) == 0)
        {
            graph->AddLine(ParseDxfLine(code, value));
        }
        else if(strcmp(value, DXF_ARC) == 0)
        {
            graph->AddLine(ParseDxfArc(code, value));
        }
        else if(strcmp(value, DXF_CIRCLE) == 0)
        {
            graph->AddLine(ParseDxfCircle(code, value));
        }
        else if(strcmp(value, DXF_EOF) == 0)
        {
            break;
        }
        else
        {
            fin.getline(code, INPUT_LINE_SIZE);
            fin.getline(value, INPUT_LINE_SIZE);
            Trim(value);
            // Currently, we don't care about other contents.
        }
    }

    fin.close();

    GraphInfo graphInfo;
    graphInfo.graph = graph;
    graphInfo.num = 1;  // TODO: How to determine the number?

    graphSet.push_back(graphInfo);

    return true;
}

Line DxfReader::ParseDxfLine(char *code, char *value)
{
    int cv;
    Line line;
    line.type = LINE;

    while(1)
    {
        fin.getline(code, INPUT_LINE_SIZE);
        fin.getline(value, INPUT_LINE_SIZE);
        cv = atoi(code);
        if(cv == DXF_COORD_X0)
        {
            line.param.lineParam.ep1.x = atof(value);
        }
        else if(cv == DXF_COORD_Y0)
        {
            line.param.lineParam.ep1.y = atof(value);
        }
        else if(cv == DXF_COORD_X1)
        {
            line.param.lineParam.ep2.x = atof(value);
        }
        else if(cv == DXF_COORD_Y1)
        {
            line.param.lineParam.ep2.y = atof(value);
        }
        else if(cv == DXF_ENTITY_TYPE)
        {
            Trim(value);
            break;
        }
    }

	return line;
}

Line DxfReader::ParseDxfCircle(char *code, char *value)
{
    int cv;
    Line line;
    line.type = CIRCLE;

    while(1)
    {
        fin.getline(code, INPUT_LINE_SIZE);
        fin.getline(value, INPUT_LINE_SIZE);
        cv = atoi(code);
        if(cv == DXF_COORD_X0)
        {
            line.param.circleParam.center.x = atof(value);
        }
        else if(cv == DXF_COORD_Y0)
        {
            line.param.circleParam.center.y = atof(value);
        }
        else if(cv == DXF_RADIUS)
        {
            line.param.circleParam.radius = atof(value);
        }
        else if(cv == DXF_ENTITY_TYPE)
        {
            Trim(value);
            break;
        }
    }

    return line;
}

Line DxfReader::ParseDxfArc(char *code, char *value)
{
    int cv;
    Line line;
    line.type = ARC;

    while(1)
    {
        fin.getline(code, INPUT_LINE_SIZE);
        fin.getline(value, INPUT_LINE_SIZE);
        cv = atoi(code);
        if(cv == DXF_COORD_X0)
        {
            line.param.arcParam.center.x = atof(value);
        }
        else if(cv == DXF_COORD_Y0)
        {
            line.param.arcParam.center.y = atof(value);
        }
        else if(cv == DXF_RADIUS)
        {
            line.param.arcParam.radius = atof(value);
        }
        else if(cv == DXF_START_ANGLE)
        {
            line.param.arcParam.startAng = atof(value);
        }
        else if(cv == DXF_END_ANGLE)
        {
            line.param.arcParam.endAng = atof(value);
        }
        else if(cv == DXF_ENTITY_TYPE)
        {
            Trim(value);
            break;
        }
    }

    return line;
}
