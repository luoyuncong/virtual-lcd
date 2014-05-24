#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include "lcdwindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void createMainWindow();
    void createLcdWindow();
public slots:
    void userBaudChanged(int state);
    void userResolutionChanged(int state);
private:
    QComboBox *baudRateComboBox;
    QComboBox *resolutionComboBox;
    LcdWindow *lcdWindow;
};

#endif // MAINWINDOW_H
