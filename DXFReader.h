#ifndef DXFREADER_H
#define DXFREADER_H

#include "IInputReader.h"


class DXFReader : public IInputReader
{
public:
    DXFReader(const char filename[], std::vector<GraphInfo> &graphSet);
    virtual ~DXFReader();

    bool ReadData();

protected:
private:
};

#endif // DXFREADER_H
