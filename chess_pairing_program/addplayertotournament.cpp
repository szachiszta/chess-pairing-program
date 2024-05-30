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

    QSqlQueryModel *modal = new QSqlQueryModel();

    QSqlQuery* qry = new QSqlQuery(mydb);
    mydb.open();

    qry->prepare("SELECT player_id, name, surname, fide_rating, date_of_birth FROM players");
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
    QString val = ui->tableView->model()->data(index).toString(); // Element, który klikniemy 2 razy
    QSqlDatabase mydb = QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName("database.db");

    if (!mydb.open()) {
        QMessageBox::warning(this, tr("Błąd"), mydb.lastError().text());
        return;
    }

    QSqlQuery qry;
    qry.prepare("SELECT * FROM players WHERE player_id=:val");
    qry.bindValue(":val", val);  // Bezpieczne zapytanie

    if (qry.exec()) {
        QString resultString;
        if (qry.next()) {
            resultString = qry.value(0).toString();  // ID zawodnika
            qDebug() << "Emitting signal with player ID:" << resultString; // Sprawdzenie emisji sygnału
            mydb.close();
            emit playerAdded(resultString);
            QMessageBox::information(this, tr("Sukces"), "Poprawnie dodano gracza o ID: " + resultString);

            // Ustawienie koloru tła dla komórki ID na zielono
            ui->tableView->model()->setData(index, QBrush(Qt::green), Qt::BackgroundRole);
            ui->tableView->viewport()->update();  // Odświeżenie widoku tabeli
        }
    } else {
        QMessageBox::warning(this, tr("Błąd"), qry.lastError().text());
    }

    mydb.close();
}


