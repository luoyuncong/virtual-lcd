#include <QDebug>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("虚拟LCD - 小老虎(luoyuncong@126.com)");
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

    QPushButton *openButton = new QPushButton("打开");

    baudRateComboBox->addItems(QStringList()<<"1200"<<"2400"<<"4800"<<"9600"<<
                       "14400"<<"19200"<<"38400"<<"57600"<<"115200"<<"230400");
    baudRateComboBox->setCurrentIndex(9);

    resolutionComboBox->addItems(QStringList()<<"320*240"<<"480*320"<<"640*480"<<"800*480");
    resolutionComboBox->setCurrentIndex(3);

    connect(userBaudRateCheck, &QCheckBox::stateChanged, this, &MainWindow::userBaudChanged);
    connect(userResolutionCheck, &QCheckBox::stateChanged, this, &MainWindow::userResolutionChanged);

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
    resize(420, 160);
}

void MainWindow::createLcdWindow()
{
    QString text = resolutionComboBox->currentText();
    QStringList list = text.split('*', QString::SkipEmptyParts);
    if(list.count() >= 2)
    {
        int w = list.at(0).toInt();
        int h = list.at(1).toInt();
        lcdWindow = new LcdWindow(w, h);
        lcdWindow->show();
    }
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
