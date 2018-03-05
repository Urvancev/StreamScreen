#ifndef MYUDP_H
#define MYUDP_H

#include <QUdpSocket>
#include <QPixmap>
#include <QWidget>
#include <QThread>

class MyUdp : public QWidget
{
    Q_OBJECT
public:
    MyUdp();

    void sendPix(QByteArray bytes, QHostAddress &host, quint16 &port);

public slots:
    void readMes();
    void findClients();
    //void readPix(char *data, unsigned short size);
    void readInit(char* data, QHostAddress &host, quint16 &port);
    void deleteClient();
    void sendBegin(QHostAddress &host, quint16 &port);
    void readBegin(QHostAddress &host, quint16 &port);
    void sendPixElement(unsigned short x, unsigned short y, QByteArray &bytes, QHostAddress &host, quint16 &port);
    void readPixElement(char *data, unsigned short size);
    //void sendKvit(QHostAddress &host, quint16 &port);

signals:
    void ready(unsigned short x, unsigned short y, QPixmap* pix);
    void send_done();
    void addr_lock();
    void newClient(unsigned char type, QHostAddress host, quint16 port);
    void BeginStream(QHostAddress &host, quint16 &port);

private:
    QUdpSocket* s;
};

#endif // MYUDP_H
