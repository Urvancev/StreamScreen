#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <paintwidget.h>
#include <myudp.h>
#include <QList>
#include "client.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void repaint();

public slots:
    void timer1();
    void stream();
    void starRecieve();
    void startStream();
    bool newClient(unsigned char type, QHostAddress host, quint16 port);

private:
    QList <client> clients;
    QThread* tread;
    QTimer* timer;
    paintWidget* w1;
    MyUdp* sock;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
