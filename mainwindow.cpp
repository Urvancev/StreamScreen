#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QTimer>
#include <QTime>
#include <QPainter>
#include <QScreen>
#include <QByteArray>
#include <QBuffer>
#include <QThread>
#include <QFile>
#include <QNetworkInterface>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->menuBar->addAction("|||");

    sock = new MyUdp;
    tread = new QThread(this);
    sock->moveToThread(tread);
    tread->start();

    connect(sock, SIGNAL(newClient(unsigned char,QHostAddress,quint16)), this, SLOT(newClient(unsigned char,QHostAddress,quint16)));

    sock->findClients();
    stream();


}

MainWindow::~MainWindow()
{
    delete ui;
    tread->quit();
    tread->~QThread();
}

bool MainWindow::newClient(unsigned char type, QHostAddress host, quint16 port) {
    QString s = host.toString().remove(0, host.toString().lastIndexOf(":") + 1);

    for (int i = 0; i < clients.length(); i++ ) {
        if (clients[i].host == host && clients[i].port == port) {
            return false;
        }
    }

    client c;
    c.type = type;
    c.host = host;
    c.port = port;
    c.info = "Client type: " + QString::number(type) + " Ip: " + s + " port = " + QString::number(port);

    clients.append(c);

    ui->listWidget->addItem(new QListWidgetItem(c.info));
}

void MainWindow::starRecieve() {
    w1 = new paintWidget();
    ui->gridLayout->addWidget(w1);

    timer = new QTimer(this);
    sock = new MyUdp;
    QThread* tread = new QThread(this);
    sock->moveToThread(tread);

    connect(timer,SIGNAL(timeout()),this,SLOT(timer1()));
    connect(this,SIGNAL(repaint()),w1,SLOT(update()));
    connect(sock,SIGNAL(ready(QPixmap*)),w1,SLOT(read(QPixmap*)));

    timer->start(40);
}

void MainWindow::startStream() {
    sock = new MyUdp;
    QThread* tread = new QThread(this);
    sock->moveToThread(tread);

    connect(timer,SIGNAL(timeout()),this,SLOT(stream()));

    tread->start();
}

void MainWindow::stream() {
    static QScreen* screen = QApplication::primaryScreen();

    static WId wid= QApplication::desktop()->winId();
    static QPixmap pix = screen->grabWindow(wid,1920);

    pix = screen->grabWindow(wid,1920);
    //pix = pix.scaled(1280,720, Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    static QByteArray bytes;
    static QBuffer buffer(&bytes);

    buffer.open(QIODevice::WriteOnly);

    pix.save(&buffer,"JPG");

    qDebug() << "before " << bytes.length();
    bytes = qCompress(bytes);
    qDebug() << "after" << bytes.length();

    sock->sendPix(bytes);
}

void MainWindow::timer1() {
    emit repaint();
}
