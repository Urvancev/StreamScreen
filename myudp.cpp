#include "myudp.h"
#include <QWidget>
#include <QPixmap>
#include <QBuffer>
#include <QFile>
#include <QTime>

struct Pix_header_t {
    unsigned int size;
};

struct initMessage {
    unsigned char type;
};

struct deleteMessage {
    char start; //D
};

MyUdp::MyUdp() {
    s = new QUdpSocket;
    s->setSocketOption(QAbstractSocket::LowDelayOption, 1);
    if (!s->bind( 9696)) {
        if (!s->bind( 9697)) {
            if (!s->bind( 9698)) {
                emit addr_lock();
            }
        }
    }
    connect(s,SIGNAL(readyRead()),this,SLOT(readMes()));
}

void MyUdp::findClients() {
    initMessage buffer;
    char start = 'I';
    buffer.type = 1;
    for (int i = 0; i < 4; i++) {
        s->writeDatagram((char*)&start,sizeof(initMessage), QHostAddress("255.255.255.255"), 9696);
        s->writeDatagram((char*)&buffer,sizeof(initMessage), QHostAddress("255.255.255.255"), 9696);
        s->writeDatagram((char*)&start,sizeof(initMessage), QHostAddress("255.255.255.255"), 9697);
        s->writeDatagram((char*)&buffer,sizeof(initMessage), QHostAddress("255.255.255.255"), 9697);
        s->writeDatagram((char*)&start,sizeof(initMessage), QHostAddress("255.255.255.255"), 9698);
        s->writeDatagram((char*)&buffer,sizeof(initMessage), QHostAddress("255.255.255.255"), 9698);
    }
}

void MyUdp::sendPix(QByteArray bytes) {
    Pix_header_t head;
    char start = 'S';
    head.size = bytes.length();
    qDebug() << head.size;
    char* p_data = bytes.data();

    s->writeDatagram((char*)&start,sizeof(char),QHostAddress::LocalHost,9001);
    s->writeDatagram((char*)&head,sizeof(Pix_header_t),QHostAddress::LocalHost,9001);
    while(head.size > 0) {
        if (head.size > 1500) {
            s->writeDatagram(p_data,1500,QHostAddress::LocalHost,9001);
            p_data += 1500;
            head.size -= 1500;
        }else {
            s->writeDatagram(p_data,head.size,QHostAddress::LocalHost,9001);
            head.size = 0;
        }
    }
    s->waitForBytesWritten(10);
    emit send_done();
}

void MyUdp::readPix() {
    unsigned int size;
    s->readDatagram((char*) &size,sizeof(unsigned int));
    char data[size];
    unsigned int pos = 0;
    if (size > 5332140) size = 0;
    else {
        while (size > pos) {
            if (s->bytesAvailable()) pos += s->readDatagram((char*)&data[pos],(size - pos));
        }
        QPixmap pix;
        if (pix.loadFromData((const unsigned char*)data,size,"JPG")) {
            emit ready(&pix);
        }
    }
}

void MyUdp::readInit() {
    unsigned char type;
    QHostAddress host;
    quint16 port;
    s->readDatagram((char*) &type, sizeof(char), &host, &port);
    if (port != 0) {
        emit newClient(type, host, port);
    }
}

void MyUdp::deleteClient() {
    //s->writeDatagram((char*)&buffer,sizeof(initMessage), QHostAddress("255.255.255.255"), 9696);
}

void MyUdp::readMes() {
    if (s->bytesAvailable()) {
        char start;
        s->readDatagram((char*) &start,sizeof(char));
        if (start == 'S') {
            readPix();
        }
        if (start == 'I') {
            readInit();
        }
    }
    if (s->bytesAvailable()) {
        readMes();
    }
}
