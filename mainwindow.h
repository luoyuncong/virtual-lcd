#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QPushButton>
#include <QCheckBox>
#include "lcdview.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void createMainWindow();
    void createLcdWindow();
    void initSerialPort();
public slots:
    void userBaudChanged(int state);
    void userResolutionChanged(int state);
    void openSerialPort();
    void showPos(int x, int y);
    void saveImage();
private:
    QComboBox *portNameComboBox;
    QComboBox *baudRateComboBox;
    QComboBox *resolutionComboBox;

    QPushButton *openButton;
    QCheckBox *userBaudRate;
    QCheckBox *userResolution;

    QWidget *lcdWindow;
    LcdView *lcdView;
    QLabel *showPosition;
};

#endif // MAINWINDOW_H
