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
    void SetPageSize(const Point &page) { this->page = page; }

protected:

private:
    void OutputScrLine(const Line &line, const Point &offset, double rot);
    void OutputScrArc(const Line &line, const Point &offset, double rot);

private:
    std::string outname;
    std::ofstream fout;
    std::vector<NestingResult> &result;
    Point page;

};

#endif // SCRWRITER_H
