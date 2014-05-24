#ifndef LCDIMAGE_H
#define LCDIMAGE_H

#include <QLabel>
#include <QImage>

class LcdView : public QLabel
{
    Q_OBJECT
public:
    LcdView(QWidget *parent = 0);
    void initLcdView();
    void setLcdSize(int w, int h);
signals:
    void showPos(int x, int y);
protected:
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
private:
    QImage lcdImage;
    int width, height;
};

#endif // LCDIMAGE_H
