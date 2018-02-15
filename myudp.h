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

    void sendPix(QByteArray bytes);

public slots:
    void run();

    void readMes();

signals:
    void ready(QPixmap* pix);
    void send_done();

private:
    QUdpSocket* s;
};

#endif // MYUDP_H
