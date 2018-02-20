#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <paintwidget.h>
#include <myudp.h>
#include <QList>
#include "client.h"
#include "clienttab.h"
#include <QHostAddress>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QHostAddress host;
    quint16 port;

signals:
    void repaintScreen();

public slots:
    void timer1();
    void stream();
    void startRecieve();
    void startStream(QHostAddress &host, quint16 &port);
    bool newClient(unsigned char type, QHostAddress host, quint16 port);
    void listRightClick(QPoint point);
    void ClientConnect(QHostAddress& host, quint16& port);

private:
    QList <client> clients;
    QThread* tread;
    QTimer* timer;
    paintWidget* w1;
    MyUdp* sock;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
