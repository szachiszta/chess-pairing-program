#include "mainwindow.h"
#include "addtournament.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(const QString &resultString,const QString &resultString2, QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    // Inicjalizacja okna głównego
    ui->setupUi(this);
    this->setWindowTitle("Program kojarzący"); // Ustawienie nowego tytułu okna

    //USTAW ID oraz nazwę aktywnego turnieju
    if (!resultString.isEmpty()) {
        setTournamentID(resultString);
        setTournamentName(resultString2);
    }

    if(tournament_id == ""){
        ui->label_active_tournament->setText("Wybierz turniej do kojarzenia");
    }else{
        ui->label_active_tournament->setText(tournament_name);
        //SPRAWDZENIE CZY SKOJARZONY TURNIEJ JEŻELI SKOJARZONY
        QSqlDatabase mydb = QSqlDatabase::addDatabase("QSQLITE");
        mydb.setDatabaseName("database.db");
        mydb.open();
        QSqlQuery qry;
        qry.prepare("SELECT * FROM games WHERE tournament_id = :val");
        qry.bindValue(":val", tournament_id);  // Bezpieczne zapytanie
        paired = false; //skojarzony
        int recordCount = 0;
        if (qry.exec()) {
            while (qry.next()) {
                recordCount++;
            }
            if (recordCount > 0) {
                paired = true;
            }
        }
        //qDebug() << "Liczba zwróconych rekordów:" << recordCount;
        //qDebug() << "Czy turniej już skojarzony:" << paired;

        //JEŻELI SKOJARZONY TURNIEJ TO WYPEŁNIJ LISTĘ GRACZY -> powiązane z listą startową
        if(paired){
            ui->paired->setText(" Skojarzony");
            QSqlQuery qry;
            qry.prepare("SELECT white_id, black_id from games where tournament_id = :tournament_id and round = 1");
            qry.bindValue(":tournament_id", tournament_id);
            if (qry.exec()) {
                while (qry.next()) {
                    QString white = qry.value(0).toString();
                    QString black = qry.value(1).toString();
                    if(white != "-1")
                        onPlayerAdded(white);
                    if(black != "-1")
                        onPlayerAdded(black);
                }
                updatePlayerTable(); // zaktualizuj listę
            }
            mydb.close();
        }
        else{
            ui->paired->setText(" Nieskojarzony");
        }

    }
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::on_pushButton_4_clicked(){
    if(tournament_id != ""){
        addPlayerToTournament *addPlayerDialog = new addPlayerToTournament(this);
        // Podłączenie sygnału przed wywołaniem exec()
        connect(addPlayerDialog, &addPlayerToTournament::playerAdded, this, &MainWindow::onPlayerAdded);
        qDebug() << "Connected playerAdded signal to onPlayerAdded slot";
        addPlayerDialog->exec(); // Wyświetlanie okna dialogowego jako modalne
        updatePlayerTable(); // Aktualizacja tabeli zawodników w turnieju aktualnym - po zamknięciu okna dodawania graczy
    }
    else{
         QMessageBox::critical(this,tr("Błąd"),"Aby dodać zawodników do turnieju musisz wybrać turniej!");
    }
}

void MainWindow::on_pushButton_3_clicked(){
    this->close();
}

void MainWindow::on_pushButton_clicked(){
    //OTWÓRZ OKNO ZARZĄDZANIA ZAWODNIKAMI
    addnewplayer addnewplayer;
    addnewplayer.setModal(true);
    addnewplayer.exec();
}


void MainWindow::on_pushButton_2_clicked(){
    //OTWÓRZ OKNO ZARZĄDZANIA TURNIEJAMI
    addtournament addtournament;
    addtournament.setModal(true);
    addtournament.exec();
}


void MainWindow::on_btn_active_tournament_clicked(){
    this->close();
    //OTWÓRZ OKNO WYBORU AKTYWNEGO TURNIEJU
    tournamentsList tournamentlist;
    tournamentlist.setModal(true);
    tournamentlist.exec();
}

void MainWindow::setTournamentID(const QString &result) {
    //Ustaw id aktywnego turnieju
    tournament_id = result;
}

void MainWindow::setTournamentName(const QString &result){
    tournament_name = result; //Nazwa turnieju - do wyświetlania
}

void MainWindow::onPlayerAdded(const QString &playerId)
{
    // Dodawanie ID zawodnika do tablicy zawodników
    if (!playerIds.contains(playerId)) {
        playerIds.append(playerId);
        qDebug() << "Player added with ID:" << playerId;
        qDebug() << "Current player IDs:" << playerIds;
    } else {
        qDebug() << "Player with ID:" << playerId << "is already added.";
    }
}


void MainWindow::updatePlayerTable(){
    //WYŚWIETLANIE WSZYSTKICH ZAWODNIKÓW DODANYCH DO TURNIEJU (w liście zawodników)
    QSqlDatabase mydb = QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName("database.db");
    mydb.open();

    QSqlQuery query;
    QSqlQueryModel * modal = new QSqlQueryModel();
    QString query_text = "SELECT name,surname, fide_rating, date_of_birth FROM players where ";

    if(playerIds.size() != 0){ //Dodaje do zapytania wszystkich zawodników z listy
        for (int i = 0; i < playerIds.size(); ++i) {
            QString playerId = playerIds.at(i);
            query_text +="player_id="+playerId;
            if(i != playerIds.size()-1)
                query_text+=" or ";
        }
    }
    modal->setQuery(query_text);
    ui->tableView->setModel(modal);
}

void MainWindow::on_btn_pairings_clicked()
{
    if(tournament_id != ""){
        if(playerIds.size()>3 || paired){ //jeżeli turniej juz byl kojarzony -> rekordy w bazie to przejdz
            pairings pairings(nullptr, playerIds, tournament_id);
            pairings.setModal(true);
            pairings.exec();
        }
        else{
            QMessageBox::critical(this,tr("Błąd"),"Aby zarządzać turniejem musisz wybrać co najmniej 4 zawodników!");
        }
    }
    else{
        QMessageBox::critical(this,tr("Błąd"),"Aby kojarzyć turniej należy wybrać turniej!");
    }
}

