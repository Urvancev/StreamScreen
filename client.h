#ifndef CLIENT_H
#define CLIENT_H

#include <QString>
#include <QHostAddress>

struct client {
    unsigned char type;
    QHostAddress host;
    quint16 port;
    QString info;
};

#endif // CLIENT_H
