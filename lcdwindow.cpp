#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include "lcdwindow.h"

LcdWindow::LcdWindow(QWidget *parent) :
    QWidget(parent)
{
    createWindow(0, 0);
}

LcdWindow::LcdWindow(int w, int h, QWidget *parent)
{
    createWindow(w, h);
}

void LcdWindow::setLcdSize(int w, int h)
{
    lcdView->setLcdSize(w, h);
}

void LcdWindow::createWindow(int w, int h)
{
    QPushButton *saveImage = new QPushButton("保存图片");
    showPosition = new QLabel("X:0, Y:0");
    showPosition->setAlignment(Qt::AlignRight);
    lcdView = new LcdView;
    lcdView->setLcdSize(w, h);
    connect(lcdView, &LcdView::showPos, this, &LcdWindow::showPos);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(saveImage);
    layout->addWidget(showPosition);
    layout->setStretch(1, 1);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(layout);
    mainLayout->addWidget(lcdView);
    mainLayout->setStretch(1, 1);
    setLayout(mainLayout);

    setFixedSize(w + 20, h + 50);
}

void LcdWindow::showPos(int x, int y)
{
    showPosition->setText("X:" + QString::number(x) + " ,Y:" + QString::number(y));
}
