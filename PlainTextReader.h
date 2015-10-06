#ifndef PLAINTEXTREADER_H
#define PLAINTEXTREADER_H

#include <string>
#include <vector>
#include <fstream>

#include "IInputReader.h"


class PlainTextReader : public IInputReader
{
public:
    PlainTextReader(const char filename[], std::vector<GraphInfo> &graphSet);
    virtual ~PlainTextReader();

    bool ReadData();

protected:
private:
    bool ReadGraph();
    LineType GetLineType(char typeStr[]);
    void ReadInputLine(char line[]);
    bool IgnoredLine(char line[], int len);

private:
    std::string filename;
    std::ifstream fin;
    std::vector<GraphInfo> &graphSet;
};

#endif // PLAINTEXTREADER_H
