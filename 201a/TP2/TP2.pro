#-------------------------------------------------
#
# Project created by QtCreator 2016-10-02T18:21:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TP2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    canvas.cpp

HEADERS  += mainwindow.h \
    canvas.h \
    drawnline.h \
    drawnshape.h \
    mode.h

FORMS    += mainwindow.ui

RESOURCES += \
    icons.qrc
