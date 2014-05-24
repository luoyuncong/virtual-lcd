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

    replace.append(ESC);
    replace.append(ESC_ESC);
    bytes.replace(ESC, replace);

    replace.clear();
    replace.append(ESC);
    replace.append(ESC_END);
    bytes.replace(END, replace);

    bytes.insert(0, END);
    bytes.append(END);

    write(bytes);
}
