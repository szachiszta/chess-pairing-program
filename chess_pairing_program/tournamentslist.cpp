#include "tournamentslist.h"
#include "ui_tournamentslist.h"
#include "mainwindow.h"
#include <QMessageBox>

tournamentsList::tournamentsList(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::tournamentsList)
{
    ui->setupUi(this);
    this->setWindowTitle("Wybierz aktywny turniej"); // Ustawienie nowego tytułu okna

    //WYŚWIETL WSZYSTKIE TURNIEJE Z BAZY DANYCH
    QSqlDatabase mydb = QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName("database.db");

    QSqlQueryModel * modal = new QSqlQueryModel();

    QSqlQuery* qry = new QSqlQuery(mydb);
    mydb.open();

    qry->prepare("SELECT id,name,place,date from tournaments");
    qry->exec();
    modal->setQuery(*qry);
    ui->tableView->setModel(modal);
    qDebug() << (modal->rowCount());
    mydb.close();
}

tournamentsList::~tournamentsList(){
    delete ui;
}


void tournamentsList::on_tableView_activated(const QModelIndex &index){

    QString val = index.sibling(index.row(), 0).data().toString(); // ID klikniętego wiersza
    QSqlDatabase mydb = QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName("database.db");
    mydb.open();


    QSqlQuery qry;
    qry.prepare("select * from tournaments where id=:val");
    qry.bindValue(":val", val);  // Bezpieczne zapytanie
    if (qry.exec()) {
        QString resultString;
        QString resultString2;
        while (qry.next()) {
            resultString = qry.value(0).toString();  //ID turnieju
            resultString2 = qry.value(3).toString(); //Nazwa turnieju
        }
        // Tworzenie i wyświetlenie okna głównego z przekazaniem wyniku zapytania
        MainWindow *mainWindow = new MainWindow(resultString, resultString2);
        mydb.close();
        this->close();
        mainWindow->show();

    } else {
        QMessageBox::warning(this, tr("Błąd"), qry.lastError().text());
    }
    mydb.close();
}

