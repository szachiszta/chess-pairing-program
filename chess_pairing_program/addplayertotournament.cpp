#include "addplayertotournament.h"
#include "ui_addplayertotournament.h"
#include "mainwindow.h"

addPlayerToTournament::addPlayerToTournament(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::addPlayerToTournament)
{
    ui->setupUi(this);

    QSqlDatabase mydb = QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName("database.db");

    QSqlQueryModel * modal = new QSqlQueryModel();

    QSqlQuery* qry = new QSqlQuery(mydb);
    mydb.open();

    qry->prepare("Select name, surname, fide_rating, date_of_birth from players");
    qry->exec();
    modal->setQuery(*qry);
    ui->tableView->setModel(modal);
    qDebug() << (modal->rowCount());
    mydb.close();
}

addPlayerToTournament::~addPlayerToTournament()
{
    delete ui;
}
