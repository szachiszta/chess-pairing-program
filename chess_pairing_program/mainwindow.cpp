#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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

