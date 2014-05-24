#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QSerialPort>

class SerialPort : public QSerialPort
{
    Q_OBJECT
public:
    SerialPort(QObject *parent = 0);
    void sendData(const char *data, qint64 maxSize);

    QByteArray buffer;
};

#endif // SERIALPORT_H
