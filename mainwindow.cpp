#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QTimer>
#include <QTime>

#include <QPainter>

#include <QScreen>
#include <paintwidget.h>

#include <myudp.h>
#include <QByteArray>
#include <QBuffer>
#include <QThread>

QTime t;
paintWidget* w1;
MyUdp* sock;
#include <QFile>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    flag = false;
    QTimer* timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timer1()));

    w1 = new paintWidget();
    ui->gridLayout->addWidget(w1);

    sock = new MyUdp;

    QThread* tread = new QThread(this);
    sock->moveToThread(tread);

    //connect(sock,SIGNAL(send_done()),this,SLOT(send_done()));
    //connect(timer,SIGNAL(timeout()),this,SLOT(stream()));

    timer->start(60);

    connect(this,SIGNAL(repaint()),w1,SLOT(update()));
    connect(sock,SIGNAL(ready(QPixmap*)),w1,SLOT(read(QPixmap*)));
    tread->start();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::send_done() {
    flag = false;
}

void MainWindow::stream() {
    static QScreen* screen = QApplication::primaryScreen();

    static WId wid= QApplication::desktop()->winId();
    static QPixmap pix = screen->grabWindow(wid,1920);

    pix = screen->grabWindow(wid,1920);
    //pix = pix.scaled(1280,720, Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    static QByteArray bytes;
    static QBuffer buffer(&bytes);

    if (flag == false) {
        flag = true;

        buffer.open(QIODevice::WriteOnly);
        pix.save(&buffer, "JPG");
        sock->sendPix(bytes);
    }
}

void MainWindow::timer1() {
    emit repaint();
}
