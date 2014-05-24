#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
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
    setFixedSize(w + 25, h + 50);
}

void LcdWindow::createWindow(int w, int h)
{
    setWindowTitle("RGB565 - 小老虎(luoyuncong@126.com)");
    QPushButton *saveImage = new QPushButton("保存图片");
    connect(saveImage, &QPushButton::clicked, this, &LcdWindow::saveImage);
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
}

void LcdWindow::showPos(int x, int y)
{
    showPosition->setText("X:" + QString::number(x) + " ,Y:" + QString::number(y));
}

void LcdWindow::saveImage()
{
    QString fileName = QFileDialog::getSaveFileName(this, "保存图片",  "./untitled.png","Images (*.png)");
    if(fileName.isEmpty())
        return;

    if(!lcdView->saveImage(fileName))
        QMessageBox::warning(this, "警告", "保存图片失败!", QMessageBox::Ok);
}
