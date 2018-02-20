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

#include <QLabel>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->menuBar->addAction("Menu");

    sock = new MyUdp;
    tread = new QThread(this);
    sock->moveToThread(tread);
    tread->start();

    timer = new QTimer(this);

    connect(sock, SIGNAL(newClient(unsigned char,QHostAddress,quint16)), this, SLOT(newClient(unsigned char,QHostAddress,quint16)));
    ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(listRightClick(QPoint)));
    connect(sock, SIGNAL(BeginStream(QHostAddress&,quint16&)), this, SLOT(startStream(QHostAddress &, quint16 &)));

    sock->findClients();
}

MainWindow::~MainWindow()
{
    delete ui;
    tread->exit();
    tread->wait();
    delete tread;
}

void MainWindow::listRightClick(QPoint point) {
    qDebug() << "click";
}

void MainWindow::ClientConnect(QHostAddress &host, quint16 &port) {
    sock->sendBegin(host, port);
    startRecieve();
}

bool MainWindow::newClient(unsigned char type, QHostAddress host, quint16 port) {
    QString s = host.toString().remove(0, host.toString().lastIndexOf(":") + 1);

    for (int i = 0; i < clients.length(); i++ ) {
        if (clients[i].host == host && clients[i].port == port) {
            return false;
        }
    }

    if (type > 4) return false;

    client c;
    c.type = type;
    c.host = host;
    c.port = port;
    c.info = "Client type: " + QString::number(type) + " Ip: " + s + " port = " + QString::number(port);

    clients.append(c);

    ClientTab* tab = new ClientTab;
    tab->setText(c.info);
    tab->type = type;
    tab->host = host;
    tab->port = port;
    tab->info = "Client type: " + QString::number(type) + " Ip: " + s + " port = " + QString::number(port);
    connect(tab, SIGNAL(clicked(QHostAddress&,quint16&)), this, SLOT(ClientConnect(QHostAddress&,quint16&)));
    QListWidgetItem* listw = new QListWidgetItem;
    listw->setSizeHint(tab->sizeHint());

    ui->listWidget->addItem(listw);
    ui->listWidget->setItemWidget(listw, tab);
    return true;
}

void MainWindow::startRecieve() {
    w1 = new paintWidget();
    w1->show();
    //ui->gridLayout->addWidget(w1);

    connect(timer,SIGNAL(timeout()),this,SLOT(timer1()));
    connect(this,SIGNAL(repaintScreen()),w1,SLOT(update()));
    connect(sock,SIGNAL(ready(QPixmap*)),w1,SLOT(read(QPixmap*)));

    timer->start(40);
}

void MainWindow::startStream(QHostAddress &host, quint16 &port) {
    this->port = port;
    this->host = host;
    connect(timer,SIGNAL(timeout()),this,SLOT(stream()));
    timer->start(40);
}

void MainWindow::stream() {
    static QScreen* screen = QApplication::primaryScreen();

    static WId wid= QApplication::desktop()->winId();
    static QPixmap pix = screen->grabWindow(wid);

    pix = screen->grabWindow(wid);
    //pix = pix.scaled(1280,720, Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    static QByteArray bytes;
    static QBuffer buffer(&bytes);

    buffer.open(QIODevice::WriteOnly);

    pix.save(&buffer,"JPG");

    qDebug() << "before " << bytes.length();
    //bytes = qCompress(bytes);
    qDebug() << "after" << bytes.length();

    sock->sendPix(bytes, this->host, this->port);
}

void MainWindow::timer1() {
    emit repaintScreen();
}
