
QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = streamScreen
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    paintwidget.cpp \
    myudp.cpp

HEADERS  += mainwindow.h \
    paintwidget.h \
    myudp.h \
    client.h

FORMS    += mainwindow.ui
