#include "mainwindow.h"
#include "addtournament.h"
#include "ui_mainwindow.h"
#include <QMessageBox>


MainWindow::MainWindow(const QString &resultString,const QString &resultString2, QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    // Inicjalizacja okna głównego
    ui->setupUi(this);
    this->setWindowTitle("Projekt program kojarzący"); // Ustawienie nowego tytułu okna

    if (!resultString.isEmpty()) {
        setTournamentID(resultString);
        setTournamentName(resultString2);
    }

    if(tournament_id == ""){
        ui->label_active_tournament->setText("Wybierz turniej do kojarzenia");
    }else{
         ui->label_active_tournament->setText(tournament_name);
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_4_clicked()
{

    if(tournament_id != ""){
        addPlayerToTournament *addPlayerDialog = new addPlayerToTournament(this);

        // Podłączenie sygnału przed wywołaniem exec()
        connect(addPlayerDialog, &addPlayerToTournament::playerAdded, this, &MainWindow::onPlayerAdded);
        qDebug() << "Connected playerAdded signal to onPlayerAdded slot";

        addPlayerDialog->exec(); // Wyświetlanie okna dialogowego jako modalne
        updatePlayerTable(); // Początkowa aktualizacja tabeli
    }
    else{
         QMessageBox::critical(this,tr("error"),"Aby dodać zawodników do turnieju musisz wybrać turniej!");
    }


}

void MainWindow::on_pushButton_3_clicked()
{
    this->close();
}

void MainWindow::on_pushButton_clicked()
{
    addnewplayer addnewplayer;
    addnewplayer.setModal(true);
    addnewplayer.exec();
}


void MainWindow::on_pushButton_2_clicked()
{
    addtournament addtournament;
    addtournament.setModal(true);
    addtournament.exec();
}


void MainWindow::on_btn_active_tournament_clicked()
{
    this->close();
    tournamentsList tournamentlist;
    tournamentlist.setModal(true);
    tournamentlist.exec();
}

void MainWindow::setTournamentID(const QString &result) {
    tournament_id = result;
}

void MainWindow::setTournamentName(const QString &result){
    tournament_name = result;
}

void MainWindow::onPlayerAdded(const QString &playerId)
{
    // Dodawanie ID zawodnika do tablicy
    if (!playerIds.contains(playerId)) {
        playerIds.append(playerId);
        qDebug() << "Player added with ID:" << playerId;
        qDebug() << "Current player IDs:" << playerIds;
    } else {
        qDebug() << "Player with ID:" << playerId << "is already added.";
    }
}


void MainWindow::updatePlayerTable()
{
    QSqlDatabase mydb = QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName("database.db");
    mydb.open();

    QSqlQuery query;
    QSqlQueryModel * modal = new QSqlQueryModel();
    QString query_text = "SELECT name,surname FROM players where ";

    if(playerIds.size() != 0){
        for (int i = 0; i < playerIds.size(); ++i) {
            QString playerId = playerIds.at(i);
            query_text +="player_id="+playerId;
            if(i != playerIds.size()-1)
                query_text+=" or ";
        }
    }//Wyświetl wszystkich dodanych zawodników

    qDebug()<<query_text;
    query.prepare(query_text);
    if (!query.exec()) {
        qDebug() << "Error fetching players from database:" << query.lastError().text();
        return;
    }
    modal->setQuery(query);
    ui->tableView->setModel(modal);
}

void MainWindow::on_btn_pairings_clicked()
{
    if(tournament_id != ""){
        if(playerIds.size()>3){ //dodaj opcję kontunuowania turnieju -> jeżeli już są kojarzenia/////////////////////////
            pairings pairings(nullptr, playerIds, tournament_id);
            pairings.setModal(true);
            pairings.exec();
        }
        else{
            QMessageBox::critical(this,tr("error"),"Aby zarządzać turniejem musisz wybrać co najmniej 4 zawodników!");
        }
    }
    else{
        QMessageBox::critical(this,tr("error"),"Aby kojarzyć turniej należy wybrać turniej!");
    }
}

