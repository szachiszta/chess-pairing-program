#include "pairings.h"
#include "ui_pairings.h"
#include "mainwindow.h"

pairings::pairings(QWidget *parent, QVector<QString> playerIds)
    : QDialog(parent)
    , ui(new Ui::pairings)
{
    ui->setupUi(this);
    QSqlDatabase mydb = QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName("database.db");

    QSqlQueryModel *modal = new QSqlQueryModel();

    QSqlQuery* qry = new QSqlQuery(mydb);
    mydb.open();


    QString query_text = "SELECT name,surname FROM players where ";

    if(playerIds.size() != 0){
        for (int i = 0; i < playerIds.size(); ++i) {
            QString playerId = playerIds.at(i);
            query_text +="player_id="+playerId;
            if(i != playerIds.size()-1)
                query_text+=" or ";
        }
    }//Wyświetl wszystkich dodanych zawodników
    qry->prepare(query_text);
    qry->exec();
    modal->setQuery(*qry);
    ui->tableView->setModel(modal);
    qDebug() << (modal->rowCount());
    mydb.close();
    mydb.removeDatabase("database.db");
}


pairings::~pairings()
{
    delete ui;
}
