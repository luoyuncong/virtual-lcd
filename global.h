#ifndef GLOBAL_H
#define GLOBAL_H
#include <QtCore>

class Global
{
#define END     (char(0xc0))
#define ESC     (char(0xdb))
#define ESC_END (char(0xdc))
#define ESC_ESC (char(0xdd))
public:
    static uchar checksum(const uchar *data, int size);
};

#endif // GLOBAL_H
