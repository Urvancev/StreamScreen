#include "clienttab.h"
#include "ui_clienttab.h"

ClientTab::ClientTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClientTab)
{
    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(clicked_f()));
}

void ClientTab::clicked_f() {
    emit clicked(host, port);
}

ClientTab::~ClientTab()
{
    delete ui;
}

void ClientTab::setText(QString& text) {
    ui->label->setText(text);
}
