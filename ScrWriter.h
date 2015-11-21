#ifndef SCRWRITER_H
#define SCRWRITER_H

#include <string>
#include <vector>
#include <fstream>

#include "IOutputWriter.h"
#include "GraphData.h"


class ScrWriter : public IOutputWriter
{
public:
    ScrWriter(const char outname[], std::vector<NestingResult> &result);
    virtual ~ScrWriter();

    bool OutputFile();

protected:

private:
    void OutputScrLine(const Line &line, const Point &offset, double angle);
    void OutputScrArc(const Line &line, const Point &offset, double angle);

private:
    std::string outname;
    std::ofstream fout;
    std::vector<NestingResult> &result;

};

#endif // SCRWRITER_H
