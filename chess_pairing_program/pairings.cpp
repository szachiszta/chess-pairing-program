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
    query_text += " order by fide_rating DESC";
    qry->prepare(query_text);
    qry->exec();
    modal->setQuery(*qry);
    ui->tableView->setModel(modal);
    //qDebug() << (modal->rowCount());

    //SPRAWDZ CZY JUŻ TURNIEJ SKOJARZONY//

    QSqlQuery qrycheck;
    qrycheck.prepare("SELECT * FROM games WHERE tournament_id = :val");
    qrycheck.bindValue(":val", tournament_idT);  // Bezpieczne zapytanie
    bool pairings_done = false; //skojarzony
    int recordCount = 0;
    if (qrycheck.exec()) {
        while (qrycheck.next()) {
            recordCount++;
        }
        if (recordCount > 0) {
            pairings_done = true;
        }
    }
    mydb.close();
    //TWORZENIE KOJARZEŃ JEŻELI NIE SKOJARZONO
    if(!pairings_done)
        generateRoundRobinPairings();

    //WYPEŁNIJ LISTĘ RUND MOŻLIWYMI RUNDAMI
    QSqlDatabase mydb2 = QSqlDatabase::addDatabase("QSQLITE");
    mydb2.setDatabaseName("database.db");
    mydb2.open();
    QSqlQuery* query_max = new QSqlQuery(mydb2);
    query_max->prepare("SELECT MAX(round) FROM games WHERE tournament_id = :tournament_id");
    query_max->bindValue(":tournament_id", tournament_idT); // Zmień na rzeczywiste ID turnieju
    if (query_max->exec() && query_max->next()) {
        int maxRound = query_max->value(0).toInt();
        for (int i = 1; i <= maxRound; ++i) {
            ui->current_round->addItem(QString::number(i));
        }
    } else {
        qDebug() << "Błąd podczas pobierania maksymalnej rundy z bazy danych:" << query_max->lastError().text();
    }
    mydb2.close();
    Update_results_table(); //Tworzenie tabeli wyników
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

    qry->prepare("INSERT INTO games (game_id, tournament_id, white_id, black_id, result, round) VALUES (:game_id, :tournament_id, :white_id, :black_id, :result, :round)");

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
            QString result = " ";
            if(whiteId == -1){
                result = "0:1"; //jeżeli pauza dla czarnych
            }
            if(blackId == -1){
                result = "1:0";
            }

            qry->bindValue(":game_id", gameId++);
            qry->bindValue(":tournament_id", tournament_idT);
            qry->bindValue(":white_id", whiteId);
            qry->bindValue(":black_id", blackId);
            qry->bindValue(":round", round + 1); //rundy numerowane od 0
            qry->bindValue(":result", result);

            if (!qry->exec()) {
                qDebug() << "Błąd podczas wstawiania danych do bazy:" << qry->lastError().text();
                return;
            }
        }
    }
    mydb.close();
}

void pairings::Show_pairings_round(int round){


}


void pairings::on_current_round_currentIndexChanged(int index)
{
    QSqlDatabase mydb = QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName("database.db");
    mydb.open();

    qDebug() << "Zmiana na" << index;
    QSqlQueryModel * modal = new QSqlQueryModel();
    // Pobierz wybraną rundę
    int selectedRound = ui->current_round->currentText().toInt();

    // Wykonaj zapytanie SQL, aby pobrać dane dla wybranej rundy
    QSqlQuery query;
    query.prepare("SELECT white.player_id, white.name || ' ' || white.surname as white, games.result, black.name || ' ' || black.surname as black, black.player_id from games inner join players AS black on games.black_id = black.player_id inner join players AS white on games.white_id = white.player_id INNER join tournaments on games.tournament_id = tournaments.id where games.tournament_id = :tournament AND round = :round");
    query.bindValue(":round", selectedRound);
    query.bindValue(":tournament", tournament_idT);

    if (query.exec()) {
        // Ustaw nowe dane dla modelu
        modal->setQuery(query);
        ui->pairings_round->setModel(modal);
    } else {
        qDebug() << "Błąd podczas pobierania danych z bazy danych:" << query.lastError().text();
    }
    mydb.close();
}


void pairings::on_current_round_activated(int index){

}


void pairings::on_results_table_activated(const QModelIndex &index)
{
 ////DO ZOBACZENIA WYNIKOW ZAWODNIKA !
 ///
    /* DO WYNIKÓW ZAWODNIKA
SELECT white.player_id, black.player_id, result from games
inner join players as white on white.player_id = games.white_id
inner join players as black on black.player_id = games.black_id
where white_id = 2 or black_id = 2
 */
}

void pairings::on_pairings_round_clicked(const QModelIndex &index){
    selected_white = index.sibling(index.row(), 0).data().toInt();
    selected_black = index.sibling(index.row(), 4).data().toInt();
}

void pairings::Add_result(QString result){
    if(selected_black != -2 && selected_white != -2){
        QSqlDatabase mydb = QSqlDatabase::addDatabase("QSQLITE");
        mydb.setDatabaseName("database.db");
        mydb.open();

        QSqlQuery query;
        query.prepare("UPDATE games SET result = :result where white_id = :white_id and black_id = :black_id and tournament_id = :tournament;");
        query.bindValue(":white_id", selected_white);
        query.bindValue(":black_id", selected_black);
        query.bindValue(":tournament", tournament_idT);
        query.bindValue(":result", result);

        if (query.exec()) {
            Update_pairings_results();
        } else {
            qDebug() << "Błąd podczas pobierania danych z bazy danych:" << query.lastError().text();
        }
        mydb.close();
    }
}



void pairings::Update_pairings_results(){
    QSqlDatabase mydb = QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName("database.db");
    mydb.open();

    QSqlQueryModel * modal = new QSqlQueryModel();
    // Pobierz wybraną rundę
    int selectedRound = ui->current_round->currentText().toInt();

    // Wykonaj zapytanie SQL, aby pobrać dane dla wybranej rundy
    QSqlQuery query;
    query.prepare("SELECT white.player_id, white.name || ' ' || white.surname as white, games.result, black.name || ' ' || black.surname as black, black.player_id from games inner join players AS black on games.black_id = black.player_id inner join players AS white on games.white_id = white.player_id INNER join tournaments on games.tournament_id = tournaments.id where games.tournament_id = :tournament AND round = :round");
    query.bindValue(":round", selectedRound);
    query.bindValue(":tournament", tournament_idT);

    if (query.exec()) {
        // Ustaw nowe dane dla modelu
        modal->setQuery(query);
        ui->pairings_round->setModel(modal);
    } else {
        qDebug() << "Błąd podczas pobierania danych z bazy danych:" << query.lastError().text();
    }
    mydb.close();
    Update_results_table();
}


void pairings::on_b_0_0_clicked(){
    QString result = "0/0";
    Add_result(result);
}

void pairings::on_b1_0_clicked(){
    QString result = "1/0";
    Add_result(result);

}


void pairings::on_b_m_p_clicked(){
    QString result = "-/+";
    Add_result(result);
}


void pairings::on_b_p_m_clicked(){
    QString result = "+/-";
    Add_result(result);
}


void pairings::on_b_0_1_clicked(){
    QString result = "0/1";
    Add_result(result);
}


void pairings::on_b_1_2_clicked(){
    QString result = "1/2";
    Add_result(result);
}


void pairings::Update_results_table(){

    QSqlDatabase mydb = QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName("database.db");

    if (!mydb.open()) {
        qDebug() << "Błąd przy otwieraniu bazy danych:" << mydb.lastError().text();
        return;
    }

    QSqlQuery query;
    QString queryString = R"(
WITH player_points AS (
    SELECT
        white_id AS player_id,
        CASE
            WHEN result = '1/0' THEN 1.0
            WHEN result = '0/1' THEN 0.0
            WHEN result = '1/2' THEN 0.5
            ELSE 0.0
        END AS points
    FROM games
    WHERE white_id != -1
    UNION ALL
    SELECT
        black_id AS player_id,
        CASE
            WHEN result = '1/0' THEN 0.0
            WHEN result = '0/1' THEN 1.0
            WHEN result = '1/2' THEN 0.5
            ELSE 0.0
        END AS points
    FROM games
    WHERE black_id != -1
)
SELECT
    p.player_id as player,
    p.name,
    p.surname,
    p.category,
    SUM(pp.points) AS total_points
FROM player_points pp
JOIN players p ON pp.player_id = p.player_id
WHERE p.player_id != -1
GROUP BY p.player_id, p.name, p.surname, p.category
ORDER BY total_points DESC
)";

    query.prepare(queryString);

    if (query.exec()) {
        QSqlQueryModel *modal = new QSqlQueryModel();
        modal->setQuery(query);
        ui->results_table->setModel(modal);  // Zakładając, że masz QTableView o nazwie tableView w interfejsie
    } else {
        qDebug() << "Błąd podczas wykonywania zapytania SQL:" << query.lastError().text();
    }

    mydb.close();


}
