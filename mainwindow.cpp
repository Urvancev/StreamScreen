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
    //stream();
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
    connect(sock,SIGNAL(ready(unsigned short, unsigned short, QPixmap*)),w1,SLOT(read(unsigned short, unsigned short, QPixmap*)));

    timer->start(1);
}

void MainWindow::startStream(QHostAddress &host, quint16 &port) {
    this->port = port;
    this->host = host;
    connect(timer,SIGNAL(timeout()),this,SLOT(stream()));
    timer->start(20);
}

void MainWindow::stream() {
    static QScreen* screen = QApplication::primaryScreen();
    static WId wid= QApplication::desktop()->winId();
    static QPixmap pix = screen->grabWindow(wid,1920);
    static QByteArray bytes;
    static QBuffer buffer(&bytes);
    static QPixmap pix_old1;
    static QPixmap pix_old2;
    static QPixmap pix_old3;
    static QPixmap pix_old4;
    static unsigned char kadr = 0;

    pix = screen->grabWindow(wid,1920);
    pix = pix.scaled(1280, 720,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);

    //static unsigned short width = screen->size().width();
    //static unsigned short height = screen->size().height();
    static unsigned short width = 1280;
    static unsigned short height = 720;
    static unsigned char iMax = height/40;
    static unsigned char jMax = width/40;

    static unsigned char block = 40;

    for (int j = 0; j < jMax; j++) {
        for (int i = 0; i < iMax; i++) {
            QPixmap pixE = pix.copy(block * j, block * i, block, block);
            QPixmap pixE_old1 = pix_old1.copy(block * j, block * i, block, block);
            QPixmap pixE_old2 = pix_old2.copy(block * j, block * i, block, block);
            QPixmap pixE_old3 = pix_old3.copy(block * j, block * i, block, block);
            QPixmap pixE_old4 = pix_old4.copy(block * j, block * i, block, block);
            if (pixE .toImage() != pixE_old1.toImage() || pixE .toImage() != pixE_old2.toImage() || pixE .toImage() != pixE_old3.toImage() || pixE .toImage() != pixE_old4.toImage() || kadr >= 10) {
                buffer.open(QIODevice::WriteOnly);
                pixE.save(&buffer,"JPG");

                //bytes = qCompress(bytes);
                sock->sendPixElement( block * j, block * i, bytes, this->host, this->port);
            }
        }
    }
    //kadr++;
    if (kadr >= 10) kadr =0;
    kadr++;
    pix_old4 = pix_old3;
    pix_old3 = pix_old2;
    pix_old2 = pix_old1;
    pix_old1 = pix;
    //qDebug() << "before " << bytes.length();
    //bytes = qCompress(bytes);
    //qDebug() << "after" << bytes.length();
    //ui->label->setText(QString::number(bytes.length()));
}

void MainWindow::timer1() {
    emit repaintScreen();
}
