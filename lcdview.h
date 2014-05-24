#ifndef LCDIMAGE_H
#define LCDIMAGE_H

#include <QLabel>
#include <QImage>
#include "global.h"
#include "serialport.h"

class LcdView : public QLabel
{
    Q_OBJECT
public:
    LcdView(QWidget *parent = 0);
    void initLcd();
    void initSerial();
    void setLcdSize(int w, int h);
    void trimLcdSize(int &x, int &y);
    void recvDataHandler(QByteArray &data);
    void setPixel(uchar *data, int size);
    void getPixel(uchar *data, int size);
    void drawHLine(uchar *data, int size);
    void drawVLine(uchar *data, int size);
    void drawBlitLine(uchar *data, int size);
    bool openSerial(const QString &name, int baudRate);
    void closeSerial();
    bool saveImage(QString &fileName);
    QRgb rgbFrom565(quint16 pixel);
    quint16 rgbTo565(QRgb color);
signals:
    void showPos(int x, int y);
public slots:
    void readyRead();
    void updateTimeout();
    void clickedTimeout();
protected:
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
private:
    SerialPort *serial;
    QTimer *updateTimer;
    QTimer *clickedTimer;
    QImage lcdImage;
    int width, height;
};

#endif // LCDIMAGE_H
