#include "mainwindow.h"
#include "addtournament.h"
#include "ui_mainwindow.h"
#include <QMessageBox>


/*MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);


    ui->label_active_tournament->setText(tournament_id);

}
*/

// W pliku źródłowym klasy MainWindow (mainwindow.cpp)
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

}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_4_clicked()
{
    addPlayerToTournament addplayertotournament;
    addplayertotournament.setModal(true);
    addplayertotournament.exec();
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



