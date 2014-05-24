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

    initLcdView();
}

void LcdView::initLcdView()
{
    setFixedSize(width, height);
    lcdImage = QImage(width, height, QImage::Format_RGB16);
    lcdImage.fill(Qt::blue);
    update();
}

void LcdView::setLcdSize(int w, int h)
{
    if(w <= 0 || h <= 0)
        return;
    width = w;
    height = h;
    initLcdView();
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

}

void LcdView::mouseReleaseEvent(QMouseEvent *event)
{

}
