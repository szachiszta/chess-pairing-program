#include "addnewplayer.h"
#include "ui_addnewplayer.h"

addnewplayer::addnewplayer(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::addnewplayer)
{
    ui->setupUi(this);
}

addnewplayer::~addnewplayer()
{
    delete ui;
}
