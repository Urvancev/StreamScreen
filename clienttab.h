#ifndef CLIENTTAB_H
#define CLIENTTAB_H

#include <QWidget>

namespace Ui {
class ClientTab;
}

class ClientTab : public QWidget
{
    Q_OBJECT

public:
    explicit ClientTab(QWidget *parent = 0);
    ~ClientTab();

private:
    Ui::ClientTab *ui;
};

#endif // CLIENTTAB_H
