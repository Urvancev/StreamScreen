#include "myudp.h"
#include <QWidget>
#include <QPixmap>
#include <QBuffer>
#include <QFile>
#include <QTime>

struct message {
    char start;
    unsigned short size;
    char data[1604];
};

struct messageS {
    char start;
    unsigned short size;
    char* data;
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
    message mes;
    mes.start = 'I';
    mes.size = 1;
    mes.data[0] = 1;
    for (int i = 0; i < 4; i++) {
        s->writeDatagram((char*)&mes, 5, QHostAddress("255.255.255.255"), 9696);
        s->writeDatagram((char*)&mes, 5, QHostAddress("255.255.255.255"), 9697);
        s->writeDatagram((char*)&mes, 5, QHostAddress("255.255.255.255"), 9698);
    }
}

void MyUdp::sendBegin(QHostAddress &host, quint16 &port) {
    char start = 'B';
    s->writeDatagram((char*)&start,sizeof(char),host, port);
}

void MyUdp::sendPixElement(unsigned short x, unsigned short y, QByteArray& bytes, QHostAddress &host, quint16 &port) {
    static message mes;
    mes.start = 'E';
    mes.size = bytes.length();
    if (mes.size <= 1600) {
        int j = 0;
        char* buf = (char*)&x;
        mes.data[j++] = buf[0];
        mes.data[j++] = buf[1];
        buf = (char*)&y;
        mes.data[j++] = buf[0];
        mes.data[j++] = buf[1];
        buf = bytes.data();
        for (int i = 0; i < mes.size; i++) {
            mes.data[j++] = buf[i];
        }
        s->writeDatagram((char*)&mes, 4 + mes.size, host, port);
        s->waitForBytesWritten(1);
    }
}
/*
void MyUdp::sendPix(QByteArray bytes, QHostAddress &host, quint16 &port) {
    char start = 'S';
    head.size = bytes.length();
    qDebug() << "send size " << head.size;
    char* p_data = bytes.data();

    s->writeDatagram((char*)&start,sizeof(char), host, port);
    s->writeDatagram((char*)&head,sizeof(Pix_header_t), host, port);
    while(head.size > 0) {
        if (head.size > 1500) {
            s->writeDatagram(p_data,1500, host, port);
            p_data += 1500;
            head.size -= 1500;
        }else {
            s->writeDatagram(p_data,head.size, host, port);
            head.size = 0;
        }
    }
    s->waitForBytesWritten(10);
    emit send_done();
}*/

void MyUdp::deleteClient() {
    //s->writeDatagram((char*)&buffer,sizeof(initMessage), QHostAddress("255.255.255.255"), 9696);
}

void MyUdp::readPixElement(char* data, unsigned short size) {
    static QPixmap pix;
    unsigned short* x = (unsigned short*) &data[0];
    unsigned short* y = (unsigned short*) &data[2];
    unsigned char* data1 = (unsigned char*)(data + 4);
    //qDebug() << "x = " << *x << "y = " << *y;

    if (pix.loadFromData((const unsigned char*)data1, size,"JPG")) {
        emit ready(*x, *y, &pix);
    }
}
/*
void MyUdp::readPix(char* data, unsigned short size) {
    s->readDatagram((char*) &size,sizeof(unsigned int));
    qDebug() << "debug size = " << size;
    if (size > 5332140) size = 0;
    unsigned int pos = 0;
    while (size > pos) {
        if (s->bytesAvailable()) pos += s->readDatagram((char*)&data[pos],(size - pos));
    }
    QPixmap pix;
    if (pix.loadFromData((const unsigned char*)data,size,"JPG")) {
        emit ready(&pix);
    }
}
*/
void MyUdp::readInit(char* data, QHostAddress &host, quint16 &port) {
    qDebug() << "readInit " << data[0];
    if (port != 0) {
        emit newClient(data[0], host, port);
    }
}

void MyUdp::readBegin(QHostAddress &host, quint16 &port) {
    emit BeginStream(host, port);
}

void MyUdp::readMes() {
    static QHostAddress host;
    static quint16 port;
    if (s->bytesAvailable()) {
        message mes;
        message* p_mes = &mes;
        qint64 mes_size = s->readDatagram((char*) p_mes,sizeof(message), &host, &port);
        if (mes.start == 'S') {
            //readPix(mes.data, mes.size);
        }
        if (mes.start == 'E') {
            readPixElement((char*)&mes.data[0], mes.size);
        }
        if (mes.start == 'I') {
            readInit(mes.data, host, port);
        }
        if (mes.start == 'B') {
            readBegin(host, port);
        }
        if (mes.start == 'K') {

        }
    }
    if (s->bytesAvailable()) {
        readMes();
    }
}
