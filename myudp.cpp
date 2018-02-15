#include "myudp.h"
#include <QWidget>
#include <QPixmap>
#include <QBuffer>
#include <QFile>

struct header_t {
    char start;
    unsigned int size;
};

MyUdp::MyUdp() {
    //this->start();
    //s->moveToThread(this);
    s = new QUdpSocket;
    s->setSocketOption(QAbstractSocket::LowDelayOption, 1);
    s->bind(QHostAddress::LocalHost, 9001);
    connect(s,SIGNAL(readyRead()),this,SLOT(readMes()));
}

void MyUdp::run() {
    s = new QUdpSocket;
    s->setSocketOption(QAbstractSocket::LowDelayOption, 1);
    s->bind(QHostAddress::LocalHost, 9001);
    connect(s,SIGNAL(readyRead()),this,SLOT(readMes()));
    //qDebug() << currentThreadId();
    //this->exec();
}

void MyUdp::sendPix(QByteArray bytes) {
    //qDebug() << bytes.length();
    header_t head;
    head.start = 'S';
    head.size = bytes.length();
    qDebug() << head.size;
    char* p_data = bytes.data();

    s->writeDatagram((char*)&head,sizeof(header_t),QHostAddress::LocalHost,9001);
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

void MyUdp::readMes() {
    //qDebug() << currentThreadId();
    if (s->bytesAvailable()) {
        header_t head;
        s->readDatagram((char*) &head,sizeof(header_t));
        //qDebug() << head.start;
        if (head.start == 'S') {
            //qDebug() << head.size;
            char data[head.size];
            unsigned int pos = 0;
            //qDebug() << head.size;
            if (head.size > 5332140) head.size = 0;
            else {
                while (head.size > pos) {
                    if (s->bytesAvailable()) pos += s->readDatagram((char*)&data[pos],(head.size - pos));
                }
                QPixmap pix;

                if (pix.loadFromData((const unsigned char*)data,head.size,"JPG")) {
                    //pix.save("test.jpg","JPG");
                    emit ready(&pix);
                }
            }
        }
    }
    if (s->bytesAvailable()) {
        readMes();
    }

}
