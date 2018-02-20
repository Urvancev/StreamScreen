#include "clienttab.h"
#include "ui_clienttab.h"

ClientTab::ClientTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClientTab)
{
    ui->setupUi(this);
}

ClientTab::~ClientTab()
{
    delete ui;
}
