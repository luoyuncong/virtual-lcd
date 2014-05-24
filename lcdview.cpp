#include <QPainter>
#include <QMouseEvent>
#include "lcdview.h"

LcdView::LcdView(QWidget *parent):
    QLabel(parent)
{
    width = 0;
    height = 0;
    setMouseTracking(true);
    setCursor(Qt::CrossCursor);
    initLcd();
    initSerial();
}

void LcdView::initLcd()
{
    setFixedSize(width, height);
    lcdImage = QImage(width, height, QImage::Format_RGB16);
    lcdImage.fill(Qt::blue);
    update();
}

void LcdView::initSerial()
{
    serial = new SerialPort(this);
    connect(serial, &SerialPort::readyRead, this, &LcdView::readyRead);

    updateTimer = new QTimer(this);
    updateTimer->setSingleShot(true);
    updateTimer->setInterval(100);
    connect(updateTimer, &QTimer::timeout, this, &LcdView::updateTimeout);

    clickedTimer = new QTimer(this);
    clickedTimer->setInterval(200);
    connect(clickedTimer, &QTimer::timeout, this, &LcdView::clickedTimeout);
}

void LcdView::setLcdSize(int w, int h)
{
    if(w <= 0 || h <= 0)
        return;
    width = w;
    height = h;
    initLcd();
}

void LcdView::trimLcdSize(int &x, int &y)
{
    if(x < 0)
        x = 0;
    if(y < 0)
        y = 0;

    if(x > width - 1)
        x = width - 1;
    if(y > height - 1)
        y = height - 1;
}

void LcdView::recvDataHandler(QByteArray &data)
{
    uchar *p = (uchar *)(data.data());
    int size = data.size();
    if(size < 1 || Global::checksum(p, size) != 0xff)
        return;

    size -= 1;
    switch(*p)
    {
    case 0x10:
        setPixel(p+1, size);
        break;
    case 0x11:
        getPixel(p+1, size);
        break;
    case 0x12:
        drawHLine(p+1, size);
    case 0x13:
        drawVLine(p+1, size);
    case 0x14:
        drawBlitLine(p+1, size);
        break;
    default:
        break;
    }

    if(!updateTimer->isActive())
        updateTimer->start();
}

void LcdView::setPixel(uchar *data, int size)
{
    if(size >= 6)
    {
        int x = data[0] + (data[1] << 8);
        int y = data[2] + (data[3] << 8);
        if(x > width - 1 || y > height - 1)
            return;
        QRgb c = rgbFrom565(data[6] + (data[7] << 8));
        lcdImage.setPixel(x, y, rgbFrom565(c));
    }
}

void LcdView::getPixel(uchar *data, int size)
{
    if(size >= 4)
    {
        int x = data[0] + (data[1] << 8);
        int y = data[2] + (data[3] << 8);
        if(x > width - 1 || y > height - 1)
            return;

        QByteArray bytes;
        bytes.append((char)0x11);
        for(int i=0; i<4; i++)
            bytes.append((char)data[i]);

        QRgb color = lcdImage.pixel(x, y);
        quint16 c = rgbTo565(color);

        bytes.append((char)c);
        bytes.append((char)(c >> 8));

        serial->sendData(bytes.constData(), bytes.size());
    }
}

void LcdView::drawHLine(uchar *data, int size)
{
    if(size >= 8)
    {
        int x1 = data[0] + (data[1] << 8);
        int x2 = data[2] + (data[3] << 8);
        int y = data[4] + (data[5] << 8);
        QRgb c = rgbFrom565(data[6] + (data[7] << 8));

        trimLcdSize(x1, y);
        trimLcdSize(x2, y);

        for(int x = x1; x <= x2; x++)
            lcdImage.setPixel(x, y, c);
    }
}

void LcdView::drawVLine(uchar *data, int size)
{
    if(size >= 8)
    {
        int x = data[0] + (data[1] << 8);
        int y1 = data[2] + (data[3] << 8);
        int y2 = data[4] + (data[5] << 8);
        QRgb c = rgbFrom565(data[6] + (data[7] << 8));

        trimLcdSize(x, y1);
        trimLcdSize(x, y2);

        for(int y = y1; y <= y2; y++)
            lcdImage.setPixel(x, y, c);
    }
}

void LcdView::drawBlitLine(uchar *data, int size)
{
//    if(size >= 8)
//    {
//        int x1 = data[0] + (data[1] << 8);
//        int x2 = data[2] + (data[3] << 8);
//        int y = data[4] + (data[5] << 8);

//        trimLcdSize(x1, y);
//        trimLcdSize(x2, y);

//        size = (size - 6)/2;
//        if(size < x2 - x1 + 1)
//            return;

//        for(int x = x1, i = 0; x <= x2; x++, i++)
//        {
//            QRgb c = rgbFrom565(data[6+2*i] + (data[6+2*i+1] << 8));
//            lcdImage.setPixel(x, y, c);
//        }
//    }
}

bool LcdView::openSerial(const QString &name, int baudRate)
{
    serial->setPortName(name);

    if(serial->open(QIODevice::ReadWrite) && serial->setDataBits(QSerialPort::Data8)
        && serial->setParity(QSerialPort::NoParity) && serial->setStopBits(QSerialPort::OneStop)
        && serial->setBaudRate(baudRate)){
        return true;
    }

    return false;
}

void LcdView::closeSerial()
{
    serial->close();
    updateTimer->stop();
    initLcd();
}

bool LcdView::saveImage(QString &fileName)
{
    return lcdImage.save(fileName, "PNG");
}

QRgb LcdView::rgbFrom565(quint16 pixel)
{
    uchar r, g, b;

    r = (pixel >> 11) & 0x1f;
    g = (pixel >> 5)  & 0x3f;
    b = pixel & 0x1f;

    return qRgb(r << 3, g << 2, b << 3);
}

quint16 LcdView::rgbTo565(QRgb color)
{
    uchar r = qRed(color);
    uchar g = qGreen(color);
    uchar b = qBlue(color);

    quint16 c = (r << 11) + (g << 5) + b;
    return c;
}

void LcdView::readyRead()
{
    char ch;
    while(serial->bytesAvailable() > 0)
    {
        serial->read(&ch, 1);
        if(ch != END)
        {
            serial->buffer.append(ch);
            continue;
        }

        for(int i=0; i<serial->buffer.size(); i++)
        {
            if(serial->buffer.at(i) == ESC)
            {
                if(i+1 > serial->buffer.size()-1)
                    return;
                if(serial->buffer.at(i+1) == ESC_END)
                {
                    serial->buffer.remove(i,2);
                    serial->buffer.insert(i,END);
                }
                else if(serial->buffer.at(i+1) == ESC_ESC)
                {
                    serial->buffer.remove(i,2);
                    serial->buffer.insert(i,ESC);
                }
            }
        }
        recvDataHandler(serial->buffer);
        serial->buffer.clear();
    }
}

void LcdView::updateTimeout()
{
    update();
}

void LcdView::clickedTimeout()
{
    QPoint pos = mapFromGlobal(QCursor::pos());
    int x = pos.x();
    int y = pos.y();

    QByteArray bytes;

    bytes.append((char)0x00);
    bytes.append((char)x);
    bytes.append((char)(x >> 8));
    bytes.append((char)y);
    bytes.append((char)(y >> 8));

    serial->sendData(bytes.constData(), bytes.size());
}

void LcdView::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.drawImage(0, 0, lcdImage);
}

void LcdView::mouseMoveEvent(QMouseEvent *event)
{
    emit showPos(event->x(), event->y());
}

void LcdView::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        int x = event->x();
        int y = event->y();

        QByteArray bytes;

        bytes.append((char)0x00);
        bytes.append((char)x);
        bytes.append((char)(x >> 8));
        bytes.append((char)y);
        bytes.append((char)(y >> 8));

        serial->sendData(bytes.constData(), bytes.size());
        clickedTimer->start();
    }
}

void LcdView::mouseReleaseEvent(QMouseEvent *event)
{
    clickedTimer->stop();
    if(event->button() == Qt::LeftButton)
    {
        int x = event->x();
        int y = event->y();

        QByteArray bytes;

        bytes.append((char)0x01);
        bytes.append((char)x);
        bytes.append((char)(x >> 8));
        bytes.append((char)y);
        bytes.append((char)(y >> 8));

        serial->sendData(bytes.constData(), bytes.size());
    }
}
