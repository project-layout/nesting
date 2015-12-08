#ifndef DXFREADER_H
#define DXFREADER_H

#include <fstream>

#include "IInputReader.h"
#include "GeneralGraph.h"


class DxfReader : public IInputReader
{
private:

public:
    DxfReader(const char filename[], std::vector<GraphInfo> &graphSet);
    virtual ~DxfReader();

    bool ReadData();

protected:

private:
    GeneralGraph::Line ParseDxfLine(char *code, char *value);
    GeneralGraph::Line ParseDxfCircle(char *code, char *value);
    GeneralGraph::Line ParseDxfArc(char *code, char *value);

private:
    std::string filename;
    std::ifstream fin;
    std::vector<GraphInfo> &graphSet;

};

#endif // DXFREADER_H
