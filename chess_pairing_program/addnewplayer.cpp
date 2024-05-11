#include "addnewplayer.h"
#include "ui_addnewplayer.h"
#include "mainwindow.h"
#include <QMessageBox>

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

void addnewplayer::on_pushButton_clicked()
{


    QString name, surname, sex,date_of_birth, federation, fide_rating, rating, category;

    name=ui->txt_name->text();
    surname=ui->txt_surname->text();
    sex=ui->txt_sex->text();
    date_of_birth=ui->txt_date->text();
    federation=ui->txt_federation->text();
    fide_rating=ui->txt_fide->text();
    rating=ui->txt_rating->text();
    category=ui->txt_category->text();

    // TO DO SPRAWDZANIE POPRAWNOŚCI PODANYCH DANYCH

    QSqlDatabase mydb = QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName("database.db");

    mydb.open();
    QSqlQuery* qry = new QSqlQuery(mydb);
    qry->prepare("INSERT into players(name,surname,sex,date_of_birth,federation,fide_rating,ranking,category) values('"+name+"', '"+surname+"', '"+sex+"' , '"+date_of_birth+"' , '"+federation+"' , '"+fide_rating+"', '"+rating+"', '"+category+"')");

    if(qry->exec()){
        QMessageBox::critical(this,tr("tt"),tr("this is a message"));
    }else{
        QMessageBox::critical(this,tr("error"),qry->lastError().text());
    }



    mydb.close();
}

