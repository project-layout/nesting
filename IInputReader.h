#ifndef IINPUTREADER_H
#define IINPUTREADER_H

#include "Graph.h"


class IInputReader
{
public:
    IInputReader();
    virtual ~IInputReader();

    virtual bool ReadData() = 0;

protected:
private:
};

#endif // IINPUTREADER_H
