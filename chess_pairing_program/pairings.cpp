#include "pairings.h"
#include "ui_pairings.h"
#include "mainwindow.h"
#include <qmessagebox.h>>

pairings::pairings(QWidget *parent, QVector<QString> playerIds,QString tournament_id)
    : QDialog(parent)
    , ui(new Ui::pairings)
{
    playerIdsT = playerIds;
    this->setWindowTitle("Turniej"); // Ustawienie nowego tytułu okna
    tournament_idT = tournament_id; // przypisz Id turnieju oraz listę graczy
    ui->setupUi(this);
    QSqlDatabase mydb = QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName("database.db");

    QSqlQueryModel *modal = new QSqlQueryModel();

    QSqlQuery* qry = new QSqlQuery(mydb);
    mydb.open();


    QString query_text = "SELECT category, name,surname, fide_rating, ranking FROM players where ";

    if(playerIds.size() != 0){
        for (int i = 0; i < playerIds.size(); ++i) {
            QString playerId = playerIds.at(i);
            query_text +="player_id="+playerId;
            if(i != playerIds.size()-1)
                query_text+=" or ";
        }
    }//Wyświetl wszystkich dodanych zawodników
    query_text += " order by fide_rating";
    qry->prepare(query_text);
    qry->exec();
    modal->setQuery(*qry);
    ui->tableView->setModel(modal);
    qDebug() << (modal->rowCount());
    mydb.close();
}


pairings::~pairings()
{
    delete ui;
}

void pairings::generateRoundRobinPairings()
{

}

void pairings::on_pushButton_clicked()
{
    QSqlDatabase mydb = QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName("database.db");
    mydb.open();

    QSqlQuery* qry = new QSqlQuery(mydb);
    qry->prepare("SELECT MAX(game_id) FROM games");
    if (!qry->exec()) {
        qDebug() << "Failed to retrieve max game_id: " << qry->lastError().text();
        mydb.close();
        QSqlDatabase::removeDatabase("generatePairingsConnection");
        return;
    }

    int maxGameId = qry->next() ? qry->value(0).toInt() : 0;
    int gameId = maxGameId + 1; // Ustaw na najwyższe game_id + 1



    qry = new QSqlQuery(mydb);
    qry->prepare("INSERT INTO games (game_id, tournament_id, white_id, black_id, result, round) VALUES ( :game_id , :tournament_id, 2 ,2, ' ' , 2)");
    qry->bindValue(":game_id", gameId);
    qry->bindValue(":tournament_id", tournament_idT);
    if(qry->exec()){
        QMessageBox::information(this,tr("success"),tr("Turniej został edytowany"));
    }else{
        QMessageBox::critical(this,tr("error"),qry->lastError().text());
    }

    mydb.close();
}

