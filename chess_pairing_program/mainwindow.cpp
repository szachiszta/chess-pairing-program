#include "mainwindow.h"
#include "addtournament.h"
#include "ui_mainwindow.h"
#include <QMessageBox>


MainWindow::MainWindow(const QString &resultString,const QString &resultString2, QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    // Inicjalizacja okna głównego
    ui->setupUi(this);

    if (!resultString.isEmpty()) {
        setTournamentID(resultString);
        setTournamentName(resultString2);
    }

    if(tournament_id == ""){
        ui->label_active_tournament->setText("Wybierz turniej do kojarzenia");
    }else{
         ui->label_active_tournament->setText(tournament_name);
    }

    updatePlayerTable(); // Początkowa aktualizacja tabeli
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_4_clicked()
{
    addPlayerToTournament *addPlayerDialog = new addPlayerToTournament(this);

    // Podłączenie sygnału przed wywołaniem exec()
    connect(addPlayerDialog, &addPlayerToTournament::playerAdded, this, &MainWindow::onPlayerAdded);
    qDebug() << "Connected playerAdded signal to onPlayerAdded slot";

    addPlayerDialog->exec(); // Wyświetlanie okna dialogowego jako modalne
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
    query.prepare("SELECT id FROM players");

    if (!query.exec()) {
        qDebug() << "Error fetching players from database:" << query.lastError().text();
        return;
    }

    playerModel->setQuery(query);

    if (playerModel->lastError().isValid()) {
        qDebug() << "Error setting model query:" << playerModel->lastError().text();
    }

    ui->tableView->resizeColumnsToContents();
}


