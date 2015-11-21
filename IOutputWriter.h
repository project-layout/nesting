#ifndef IOUTPUTWRITER_H
#define IOUTPUTWRITER_H


class IOutputWriter
{
public:
    IOutputWriter();
    virtual ~IOutputWriter();

    virtual bool OutputFile() = 0;

protected:
private:
};

#endif // IOUTPUTWRITER_H
