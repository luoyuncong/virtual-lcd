#include <QDebug>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QFileDialog>
#include <QMessageBox>
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("虚拟LCD - 作者：小老虎");
    createMainWindow();
    createLcdWindow();
}

MainWindow::~MainWindow()
{
}

void MainWindow::createMainWindow()
{
    QWidget *window = new QWidget;
    QLabel *portNameLabel = new QLabel("串口号：");
    QLabel *baudRateLabel = new QLabel("波特率：");
    QLabel *resolutionLabel = new QLabel("分辨率：");

    QComboBox *portCombobox = new QComboBox;
    baudRateComboBox = new QComboBox;
    resolutionComboBox = new QComboBox;

    QCheckBox *userBaudRateCheck = new QCheckBox("自定义");
    QCheckBox *userResolutionCheck = new QCheckBox("自定义");
    connect(userBaudRateCheck, &QCheckBox::stateChanged, this, &MainWindow::userBaudChanged);
    connect(userResolutionCheck, &QCheckBox::stateChanged, this, &MainWindow::userResolutionChanged);

    QPushButton *openButton = new QPushButton("打开");
    connect(openButton, &QPushButton::clicked, this, &MainWindow::openSerialPort);

    baudRateComboBox->addItems(QStringList()<<"1200"<<"2400"<<"4800"<<"9600"<<
                       "14400"<<"19200"<<"38400"<<"57600"<<"115200"<<"230400");
    baudRateComboBox->setCurrentIndex(9);

    resolutionComboBox->addItems(QStringList()<<"320*240"<<"480*320"<<"640*480"<<"800*480");
    resolutionComboBox->setCurrentIndex(3);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(portNameLabel, 0, 0);
    layout->addWidget(portCombobox, 0, 1);
    layout->addWidget(openButton, 0, 2);
    layout->addWidget(baudRateLabel, 1, 0);
    layout->addWidget(baudRateComboBox, 1, 1);
    layout->addWidget(userBaudRateCheck, 1, 2);
    layout->addWidget(resolutionLabel, 2, 0);
    layout->addWidget(resolutionComboBox, 2, 1);
    layout->addWidget(userResolutionCheck, 2, 2);
    layout->setColumnStretch(1, 1);
    window->setLayout(layout);

    setCentralWidget(window);
    resize(320, 160);
}

void MainWindow::createLcdWindow()
{
    lcdWindow = new QWidget;

    lcdWindow->setWindowTitle("RGB565 - 小老虎(luoyuncong@126.com)");
    QPushButton *saveImage = new QPushButton("保存图片");
    connect(saveImage, &QPushButton::clicked, this, &MainWindow::saveImage);
    showPosition = new QLabel("X:0, Y:0");
    showPosition->setAlignment(Qt::AlignRight);
    lcdView = new LcdView;
    lcdView->setLcdSize(800, 480);
    connect(lcdView, &LcdView::showPos, this, &MainWindow::showPos);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(saveImage);
    layout->addWidget(showPosition);
    layout->setStretch(1, 1);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(layout);
    mainLayout->addWidget(lcdView);
    mainLayout->setStretch(1, 1);
    lcdWindow->setLayout(mainLayout);
}

void MainWindow::userBaudChanged(int state)
{
    if(state == Qt::Checked)
        baudRateComboBox->setEditable(true);
    else
        baudRateComboBox->setEditable(false);
    baudRateComboBox->update();
}

void MainWindow::userResolutionChanged(int state)
{
    if(state == Qt::Checked)
        resolutionComboBox->setEditable(true);
    else
        resolutionComboBox->setEditable(false);
    resolutionComboBox->update();
}

void MainWindow::openSerialPort()
{
    QString text = resolutionComboBox->currentText();
    QStringList list = text.split('*', QString::SkipEmptyParts);
    if(list.count() >= 2)
    {
        int w = list.at(0).toInt();
        int h = list.at(1).toInt();
        lcdView->setLcdSize(w, h);
        lcdWindow->setFixedSize(w + 23, h + 50);
        lcdWindow->show();
    }
}

void MainWindow::showPos(int x, int y)
{
    showPosition->setText("X:" + QString::number(x) + " ,Y:" + QString::number(y));
}

void MainWindow::saveImage()
{
    QString fileName = QFileDialog::getSaveFileName(this, "保存图片",  "./untitled.png","Images (*.png)");
    if(fileName.isEmpty())
        return;

    if(!lcdView->saveImage(fileName))
        QMessageBox::warning(this, "警告", "保存图片失败!", QMessageBox::Ok);
}
