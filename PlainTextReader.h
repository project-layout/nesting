#ifndef PLAINTEXTREADER_H
#define PLAINTEXTREADER_H

#include <string>
#include <vector>
#include <fstream>

#include "IInputReader.h"
#include "GeneralGraph.h"

class PlainTextReader : public IInputReader
{
public:
    PlainTextReader(const char filename[], std::vector<GraphInfo> &graphSet);
    virtual ~PlainTextReader();

    bool ReadData();

protected:
private:
    bool ReadGraph();
    GeneralGraph::LineType GetLineType(char typeStr[]);
    void ReadInputLine(char line[], int len);
    bool IgnoredLine(char line[], int len);

private:
    std::string filename;
    std::ifstream fin;
    std::vector<GraphInfo> &graphSet;
};

#endif // PLAINTEXTREADER_H
