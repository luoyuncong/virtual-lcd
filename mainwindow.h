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
    void createSettingDialog();
    void createLcdWindow();
    void initSerialPort();
public slots:
    void userBaudChanged(int state);
    void userResolutionChanged(int state);
    void openSerialPort();
    void showPos(int x, int y);
    void saveImage();
    void showSettingDialog();
private:
    QComboBox *portNameComboBox;
    QComboBox *baudRateComboBox;
    QComboBox *resolutionComboBox;

    QPushButton *openButton;
    QCheckBox *userBaudRate;
    QCheckBox *userResolution;

    QWidget *lcdWindow;
    QDialog *settingDialog;
    LcdView *lcdView;
    QLabel *showPosition;
};

#endif // MAINWINDOW_H
