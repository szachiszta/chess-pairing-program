#include "addtournament.h"
#include "ui_addtournament.h"
#include "mainwindow.h"
#include <QMessageBox>

addtournament::addtournament(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::addtournament)
{
    ui->setupUi(this);
}

addtournament::~addtournament()
{
    delete ui;
}

void addtournament::on_pushButton_2_clicked()
{
    this->close();
}


void addtournament::on_add_tournament_clicked()
{
    QString arbiter, place, name, number_of_rounds, date;

    arbiter=ui->txt_arbiter->text();
    place=ui->txt_place->text();
    name=ui->txt_name->text();
    number_of_rounds=ui->txt_number->text();
    date=ui->txt_date->text();


    // TO DO SPRAWDZANIE POPRAWNOŚCI PODANYCH DANYCH

    QSqlDatabase mydb = QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName("database.db");

    mydb.open();
    QSqlQuery* qry = new QSqlQuery(mydb);
    qry->prepare("INSERT into tournaments(arbiter,place,name,number_of_rounds,date) values('"+arbiter+"', '"+place+"', '"+name+"' , '"+number_of_rounds+"' , '"+date+"')");

    if(qry->exec()){
        QMessageBox::information(this,tr("success"),tr("Turniej został dodany"));
        this->close();
    }else{
        QMessageBox::critical(this,tr("error"),qry->lastError().text());
    }
    mydb.close();
}

