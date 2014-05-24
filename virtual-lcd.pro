#-------------------------------------------------
#
# Project created by QtCreator 2014-05-24T12:51:46
#
#-------------------------------------------------

QT       += core gui
QT  += serialport
#DEFINES += QT_NO_DEBUG_OUTPUT

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = virtual-lcd
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    lcdview.cpp \
    serialport.cpp \
    global.cpp

HEADERS  += mainwindow.h \
    lcdview.h \
    serialport.h \
    global.h
