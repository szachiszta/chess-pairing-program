#include "playerresults.h"
#include "ui_playerresults.h"


PlayerResults::PlayerResults(QWidget *parent, int PlayerID, QString TournamentID)
    : QDialog(parent)
    , ui(new Ui::PlayerResults)
{
    ui->setupUi(this);
    this->setWindowTitle("Wyniki zawodnika"); // Ustawienie nowego tytułu okna

    //Zapisanie player id i tournament id
    Player_id = PlayerID;
    Tournament_id = TournamentID;

    QSqlDatabase mydb = QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName("database.db");

    // Sprawdzenie, czy baza danych została otwarta
    if (!mydb.open()) {
        QMessageBox::warning(this, tr("Błąd"), tr("Nie można otworzyć bazy danych: ") + mydb.lastError().text());
        return;
    }

    QSqlQuery* qry = new QSqlQuery(mydb);
    qry->prepare("SELECT * FROM players WHERE player_id = :player");
    qry->bindValue(":player", Player_id);

    if (qry->exec()) {
        if (qry->next()) {  // Przejdź do pierwszego wyniku
            // USTAWIENIE PÓL FORMULARZA (wynik kwerendy)
            ui->txt_name->setText(qry->value(1).toString());
            ui->txt_surname->setText(qry->value(2).toString());
            ui->txt_sex->setCurrentText(qry->value(3).toString());
            QDate parsedDate = QDate::fromString(qry->value(4).toString(), "dd.MM.yyyy");
            ui->txt_date->setDate(parsedDate);
            ui->txt_federation->setText(qry->value(5).toString());
            ui->txt_fide->setText(qry->value(6).toString());
            ui->txt_rating->setText(qry->value(7).toString());
            ui->txt_category->setText(qry->value(8).toString());
        } else {
            QMessageBox::warning(this, tr("Błąd"), tr("Nie znaleziono danych zawodnika."));
        }
    } else {
        QMessageBox::warning(this, tr("Błąd"), tr("Nie udało się wykonać zapytania: ") + qry->lastError().text());
    }
    mydb.close();

    //ui->table_player_results;


    ShowStatistics();
}

PlayerResults::~PlayerResults(){
    delete ui;
}

void PlayerResults::ShowStatistics(){
    QSqlDatabase mydb = QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName("database.db");

    if (!mydb.open()) {
        qDebug() << "Błąd przy otwieraniu bazy danych:" << mydb.lastError().text();
        return;
    }

    QSqlQuery query;
    //Kwerenda licząca liczbę punktów każdego zawodnika
    QString queryString = R"(
    SELECT
        CASE WHEN white_id = 1 THEN black_id ELSE white_id END AS oponent_id,
        CASE WHEN white_id = 1 THEN 'białe' ELSE 'czarne' END AS color,
        players.category,
        players.name,
        players.surname,
        players.fide_rating,
        result,
        round
            FROM games
                INNER JOIN players ON players.player_id =
        CASE
            WHEN games.white_id = 1 THEN games.black_id
              ELSE games.white_id
                  END
)";
    queryString += "WHERE (games.black_id ="+QString::number(Player_id)+" OR games.white_id ="+QString::number(Player_id)+" ) AND games.tournament_id ="+Tournament_id+";";
    query.prepare(queryString);

    if (query.exec()) {
        QSqlQueryModel *modal = new QSqlQueryModel();
        modal->setQuery(query);
        ui->table_player_results->setModel(modal);  // Zakładając, że masz QTableView o nazwie tableView w interfejsie
    } else {
        qDebug() << "Błąd podczas wykonywania zapytania SQL:" << query.lastError().text();
    }
    mydb.close();
}


