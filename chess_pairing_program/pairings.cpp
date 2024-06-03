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

    //LISTA STARTOWA ZAWODNIKÓW
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
    //qDebug() << (modal->rowCount());

    //SPRAWDZ CZY JUŻ TURNIEJ SKOJARZONY//

    QSqlQuery qrycheck;
    qrycheck.prepare("SELECT * FROM tournaments where id=:val");
    qrycheck.bindValue(":val", tournament_id);  // Bezpieczne zapytanie
    bool pairings_done = false; //skojarzony
    if (qrycheck.exec()) {
        if(qrycheck.size() != 0) //jeżeli turniej już skojarzony{
            pairings_done = true;
    }
    mydb.close();

    //if(!pairings_done)
        generateRoundRobinPairings();
}

pairings::~pairings()
{
    delete ui;
}

void pairings::generateRoundRobinPairings()
{
    QSqlDatabase mydb = QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName("database.db");
    mydb.open();

    QSqlQuery* qry = new QSqlQuery(mydb);
    qry->prepare("SELECT MAX(game_id) FROM games");
    if (!qry->exec()) {
        qDebug() << "Failed to retrieve max game_id: " << qry->lastError().text();
        mydb.close();
        return;
    }

    int maxGameId = qry->next() ? qry->value(0).toInt() : 0;
    int gameId = maxGameId + 1; // Ustaw na najwyższe game_id + 1

    QVector<QString> players = playerIdsT;
    int numPlayers = players.size();
    bool isOdd = (numPlayers % 2 != 0);

    if (isOdd) {
        players.push_back("bye"); // Dodaj "bye", jeśli liczba zawodników jest nieparzysta
        numPlayers++;
    }

    int numRounds = numPlayers - 1;

    qry->prepare("INSERT INTO games (game_id, tournament_id, white_id, black_id, result, round) VALUES (:game_id, :tournament_id, :white_id, :black_id, ' ', :round)");

    for (int round = 0; round < numRounds; ++round) {
        for (int i = 0; i < numPlayers / 2; ++i) {
            int white = (round + i) % (numPlayers - 1);
            int black = (numPlayers - 1 - i + round) % (numPlayers - 1);

            if (i == 0) {
                black = numPlayers - 1;
            }

            QString whitePlayer = players[white];
            QString blackPlayer = players[black];

            // Jeśli jeden z zawodników jest "bye", przypisujemy ID jako -1
            int whiteId = (whitePlayer == "bye") ? -1 : whitePlayer.toInt();
            int blackId = (blackPlayer == "bye") ? -1 : blackPlayer.toInt();

            qry->bindValue(":game_id", gameId++);
            qry->bindValue(":tournament_id", tournament_idT);
            qry->bindValue(":white_id", whiteId);
            qry->bindValue(":black_id", blackId);
            qry->bindValue(":round", round + 1); //rundy numerowane od 0

            if (!qry->exec()) {
                qDebug() << "Błąd podczas wstawiania danych do bazy:" << qry->lastError().text();
                return;
            }
        }
    }
    mydb.close();
}



