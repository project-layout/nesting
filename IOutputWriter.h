#ifndef IOUTPUTWRITER_H
#define IOUTPUTWRITER_H

#include "GraphData.h"


class IOutputWriter
{
public:
    IOutputWriter();
    virtual ~IOutputWriter();

    virtual bool OutputFile() = 0;
    virtual void SetPageSize(const Point &page) = 0;

protected:
private:
};

#endif // IOUTPUTWRITER_H
