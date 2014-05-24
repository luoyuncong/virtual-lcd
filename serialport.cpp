#include "global.h"
#include "serialport.h"

SerialPort::SerialPort(QObject *parent) :
    QSerialPort(parent)
{
    buffer.clear();
}

void SerialPort::sendData(const char *data, qint64 maxSize)
{
    if(!isOpen())
        return;

    QByteArray bytes(data, maxSize);
    QByteArray replace;

    quint8 sum = ~ Global::checksum((const uchar *)data, maxSize);
    bytes.append((char)sum);

    replace.append((char)0xdb);
    replace.append((char)0xdd);
    bytes.replace((char)0xdb, replace);

    replace.clear();
    replace.append((char)0xdb);
    replace.append((char)0xdc);
    bytes.replace((char)0xc0, replace);

    bytes.insert(0, (char)0xc0);
    bytes.append((char)0xc0);

    write(bytes);
}
