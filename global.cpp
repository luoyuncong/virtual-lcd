#include "global.h"

uchar Global::checksum(const uchar *data, int size)
{
    uchar sum = 0;
    for(int i=0; i<size; i++)
        sum += *(data + i);
    return sum;
}
