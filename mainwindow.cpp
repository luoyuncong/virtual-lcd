#include <QDebug>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QSerialPortInfo>
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("RGB565 - 作者：小老虎(luoyuncong@126.com)");
    createSettingDialog();
    createLcdWindow();
    initSerialPort();
}

MainWindow::~MainWindow()
{
}

void MainWindow::createSettingDialog()
{
    settingDialog = new QDialog;
    settingDialog->setWindowTitle("设置参数");

    QLabel *portNameLabel = new QLabel("串口号：");
    QLabel *baudRateLabel = new QLabel("波特率：");
    QLabel *resolutionLabel = new QLabel("分辨率：");

    portNameComboBox = new QComboBox;
    baudRateComboBox = new QComboBox;
    resolutionComboBox = new QComboBox;

    userBaudRate = new QCheckBox("自定义");
    userResolution = new QCheckBox("自定义");
    connect(userBaudRate, &QCheckBox::stateChanged, this, &MainWindow::userBaudChanged);
    connect(userResolution, &QCheckBox::stateChanged, this, &MainWindow::userResolutionChanged);

    openButton = new QPushButton("打开");
    connect(openButton, &QPushButton::clicked, this, &MainWindow::openSerialPort);

    baudRateComboBox->addItems(QStringList()<<"1200"<<"2400"<<"4800"<<"9600"<<
                       "14400"<<"19200"<<"38400"<<"57600"<<"115200"<<"230400");
    baudRateComboBox->setCurrentIndex(9);

    resolutionComboBox->addItems(QStringList()<<"320*240"<<"480*320"<<"640*480"<<"800*480");
    resolutionComboBox->setCurrentIndex(3);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(portNameLabel, 0, 0);
    layout->addWidget(portNameComboBox, 0, 1);
    layout->addWidget(openButton, 0, 2);
    layout->addWidget(baudRateLabel, 1, 0);
    layout->addWidget(baudRateComboBox, 1, 1);
    layout->addWidget(userBaudRate, 1, 2);
    layout->addWidget(resolutionLabel, 2, 0);
    layout->addWidget(resolutionComboBox, 2, 1);
    layout->addWidget(userResolution, 2, 2);
    layout->setColumnStretch(1, 1);
    settingDialog->setLayout(layout);
    settingDialog->setModal(true);
    settingDialog->resize(320, 160);
}

void MainWindow::createLcdWindow()
{
    lcdWindow = new QWidget;

    QPushButton *settingButton = new QPushButton("参数设置");
    connect(settingButton, &QPushButton::clicked, this, &MainWindow::showSettingDialog);
    QPushButton *saveImage = new QPushButton("保存图片");
    connect(saveImage, &QPushButton::clicked, this, &MainWindow::saveImage);

    showPosition = new QLabel("X:0, Y:0");
    showPosition->setAlignment(Qt::AlignRight);
    lcdView = new LcdView;
    lcdView->setLcdSize(800, 480);
    connect(lcdView, &LcdView::showPos, this, &MainWindow::showPos);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(settingButton);
    layout->addWidget(saveImage);
    layout->addWidget(showPosition);
    layout->setStretch(2, 1);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(layout);
    mainLayout->addWidget(lcdView);
    mainLayout->setStretch(1, 1);
    lcdWindow->setLayout(mainLayout);
    setCentralWidget(lcdWindow);
}

void MainWindow::initSerialPort()
{
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    for (int i = 0; i < ports.count(); i++)
        portNameComboBox->addItem(ports.at(i).portName());
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
    if(openButton->text() == "打开")
    {
        if(lcdView->openSerial(portNameComboBox->currentText(), baudRateComboBox->currentText().toInt()))
        {
            openButton->setText("关闭");
            portNameComboBox->setEnabled(false);
            baudRateComboBox->setEnabled(false);
            resolutionComboBox->setEnabled(false);
            userBaudRate->setEnabled(false);
            userResolution->setEnabled(false);
            QString text = resolutionComboBox->currentText();
            QStringList list = text.split('*', QString::SkipEmptyParts);
            if(list.count() >= 2)
            {
                int w = list.at(0).toInt();
                int h = list.at(1).toInt();
                lcdView->setLcdSize(w, h);
                setFixedSize(w + 23, h + 50);
            }
        }
    }
    else
    {
        lcdView->closeSerial();
        openButton->setText("打开");
        portNameComboBox->setEnabled(true);
        baudRateComboBox->setEnabled(true);
        resolutionComboBox->setEnabled(true);
        userBaudRate->setEnabled(true);
        userResolution->setEnabled(true);
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

void MainWindow::showSettingDialog()
{
    settingDialog->show();
}
