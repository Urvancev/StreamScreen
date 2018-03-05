
QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = streamScreen
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    paintwidget.cpp \
    myudp.cpp \
    clienttab.cpp

HEADERS  += mainwindow.h \
    paintwidget.h \
    myudp.h \
    client.h \
    clienttab.h

FORMS    += mainwindow.ui \
    clienttab.ui

#LIBS += -lglu32 -lopengl32
