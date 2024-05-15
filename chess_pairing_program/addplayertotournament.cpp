#include "addplayertotournament.h"
#include "ui_addplayertotournament.h"
#include "mainwindow.h"
#include <QMessageBox>

addPlayerToTournament::addPlayerToTournament(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::addPlayerToTournament)
{
    ui->setupUi(this);

    QSqlDatabase mydb = QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName("database.db");

    QSqlQueryModel * modal = new QSqlQueryModel();

    QSqlQuery* qry = new QSqlQuery(mydb);
    mydb.open();

    qry->prepare("Select player_id, name, surname, fide_rating, date_of_birth from players");
    qry->exec();
    modal->setQuery(*qry);
    ui->tableView->setModel(modal);
    qDebug() << (modal->rowCount());
    mydb.close();
}

addPlayerToTournament::~addPlayerToTournament()
{
    delete ui;
}

void addPlayerToTournament::on_tableView_activated(const QModelIndex &index)
{
// TO DO dodawanie tylko za pomocą ID -> być moze fix

    QString val=ui->tableView->model()->data(index).toString(); //element który klikniemy 2 razy
    QSqlDatabase mydb = QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName("database.db");

    if (!mydb.open()) {
        QMessageBox::warning(this, tr("Błąd"), mydb.lastError().text());
        return;
    }


    QSqlQuery qry;
    qry.prepare("select * from players where player_id='"+val+"'"); //uniwersalne dla każdej klikniętej wartości
    qry.bindValue(":val", val);  // Bezpieczne zapytanie

    if (qry.exec()) {
        QString resultString;
        while (qry.next()) {
            resultString = qry.value(0).toString();  // ID zawodnika
            qDebug() << "Emitting signal with player ID:" << resultString; //Sprawdzenie emisji sygnału
            emit playerAdded(resultString);
            QMessageBox::information(this,tr("Sukces"),"Poprawnie dodano gracza o ID: "+resultString);
        }
        // Tworzenie i wyświetlenie okna głównego z przekazaniem wyniku zapytania
    } else {
        QMessageBox::warning(this, tr("Błąd"), qry.lastError().text());
    }

    mydb.close();

}

