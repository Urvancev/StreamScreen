#ifndef CLIENTTAB_H
#define CLIENTTAB_H

#include <QWidget>
#include <QHostAddress>

namespace Ui {
class ClientTab;
}

class ClientTab : public QWidget
{
    Q_OBJECT

public:
    explicit ClientTab(QWidget *parent = 0);
    ~ClientTab();

    unsigned char type;
    QHostAddress host;
    quint16 port;
    QString info;

public slots:
    void setText(QString &text);
    void clicked_f();

signals:
    void clicked(QHostAddress& host, quint16& port);

private:
    Ui::ClientTab *ui;
};

#endif // CLIENTTAB_H
