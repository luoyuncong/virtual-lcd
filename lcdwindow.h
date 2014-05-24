#ifndef LCDWINDOW_H
#define LCDWINDOW_H

#include <QWidget>
#include <QLabel>
#include "lcdview.h"

class LcdWindow : public QWidget
{
    Q_OBJECT
public:
    LcdWindow(QWidget *parent = 0);
    LcdWindow(int w, int h, QWidget *parent = 0);
    void setLcdSize(int w, int h);
    void createWindow(int w, int h);
signals:

public slots:
    void showPos(int x, int y);
private:
    LcdView *lcdView;
    QLabel *showPosition;
};

#endif // LCDWINDOW_H
