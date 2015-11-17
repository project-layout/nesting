#ifndef DXFREADER_H
#define DXFREADER_H

#include <fstream>

#include "IInputReader.h"
#include "GraphData.h"


class DxfReader : public IInputReader
{
private:

public:
    DxfReader(const char filename[], std::vector<GraphInfo> &graphSet);
    virtual ~DxfReader();

    bool ReadData();

protected:

private:
    Line ParseDxfLine(char *code, char *value);
    Line ParseDxfCircle(char *code, char *value);
    Line ParseDxfArc(char *code, char *value);

private:
    std::string filename;
    std::ifstream fin;
    std::vector<GraphInfo> &graphSet;

};

#endif // DXFREADER_H
