#include "addtournament.h"
#include "ui_addtournament.h"
#include "mainwindow.h"
#include <QMessageBox>

addtournament::addtournament(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::addtournament)
{
    ui->setupUi(this);
    this->setWindowTitle("Zarządzanie turniejami"); // Ustawienie nowego tytułu okna

    //WYPISANIE TURNIEJÓW Z BAZY DANYCH
    QSqlDatabase mydb = QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName("database.db");
    QSqlQueryModel * modal = new QSqlQueryModel(); // do wpisania wartości do table view
    QSqlQuery* qry = new QSqlQuery(mydb);
    mydb.open();
    qry->prepare("SELECT id,name,place,date,number_of_rounds as rounds from tournaments");
    qry->exec();
    modal->setQuery(*qry);
    ui->tableView->setModel(modal);
    qDebug() << (modal->rowCount());
    mydb.close();
    mydb.removeDatabase("database.db");
}

addtournament::~addtournament(){
    delete ui;
}

void addtournament::on_pushButton_2_clicked(){
    this->close();
}


void addtournament::on_add_tournament_clicked()
{
    QString arbiter, place, name, number_of_rounds, date;

    arbiter=ui->txt_arbiter->text();
    place=ui->txt_place->text();
    name=ui->txt_name->text();
    number_of_rounds=ui->txt_number->text();
    date=ui->txt_date->text();
    // TO DO SPRAWDZANIE POPRAWNOŚCI PODANYCH DANYCH
    if(checkTournamentInputs(arbiter,place, name, number_of_rounds)){
        QSqlDatabase mydb = QSqlDatabase::addDatabase("QSQLITE");
        mydb.setDatabaseName("database.db");

        mydb.open();
        QSqlQuery* qry = new QSqlQuery(mydb);
        qry->prepare("INSERT into tournaments(arbiter,place,name,number_of_rounds,date) values('"+arbiter+"', '"+place+"', '"+name+"' , '"+number_of_rounds+"' , '"+date+"')");

        if(qry->exec()){
            QMessageBox::information(this,tr("success"),tr("Turniej został dodany"));
            this->close();
            addtournament *newDialog = new addtournament();
            newDialog->show();
        }else{
            QMessageBox::critical(this,tr("Błąd"),qry->lastError().text());
        }
        mydb.close(); // Zamknięcie połączenia z bazą danych
    }
}


void addtournament::on_tableView_clicked(const QModelIndex &index){
    // FUNKCJA WYPEŁNIAJĄCA DANE Z FORMULARZA PO KLIKNIĘCIU DOWOLNEJ KOMÓRKI//
    manage_tournament = index.sibling(index.row(), 0).data().toInt(); //pobiera id dowolnego wiersza
    QSqlDatabase mydb = QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName("database.db");

    // Sprawdzenie, czy baza danych została otwarta
    if (!mydb.open()) {
        QMessageBox::warning(this, tr("Błąd"), tr("Nie można otworzyć bazy danych: ") + mydb.lastError().text());
        return;
    }
    QSqlQuery qry;
    qry.prepare("SELECT * FROM tournaments where id=:val");
    qry.bindValue(":val", manage_tournament);

    if (qry.exec()) {
        QString resultString;
        if (qry.next()) {
            resultString = qry.value(0).toString();  // ID zawodnika
        }
        //UZUPEŁNIANIE FORMULARZA
        ui->txt_arbiter->setText(qry.value(1).toString());
        ui->txt_place->setText(qry.value(2).toString());
        ui->txt_name->setText(qry.value(3).toString());
        ui->txt_number->setText(qry.value(4).toString());
        QDate parsedData = QDate::fromString(qry.value(5).toString(), "dd.MM.yyyy");
        ui->txt_date->setDate(parsedData);
    } else {
        QMessageBox::warning(this, tr("Błąd"), tr("Nie udało się wykonać zapytania: ") + qry.lastError().text());
    }
    mydb.close();
}


void addtournament::on_edit_tournament_clicked()
{
    if(manage_tournament != -1){ //Jeżeli turniej wybrany możesz edytować
        //POBRANIE WARTOŚCI Z FORMULARZA
        QString arbiter, place, name, number_of_rounds, date;
        arbiter=ui->txt_arbiter->text();
        place=ui->txt_place->text();
        name=ui->txt_name->text();
        number_of_rounds=ui->txt_number->text();
        date=ui->txt_date->text();
        //SPRAWDZANIE POPRAWNOŚCI PODANYCH DANYCH
        if(checkTournamentInputs(arbiter,place, name, number_of_rounds)){
            QSqlDatabase mydb = QSqlDatabase::addDatabase("QSQLITE");
            mydb.setDatabaseName("database.db");
            mydb.open();
            QSqlQuery* qry = new QSqlQuery(mydb);
            //AKTUALIZACJA DANYCH
            qry->prepare("UPDATE tournaments SET arbiter = :arbiter, place = :place, name = :name, number_of_rounds = :number_of_rounds, date = :date WHERE id = :manage_tournament");
            qry->bindValue(":arbiter", arbiter);
            qry->bindValue(":place", place);
            qry->bindValue(":name", name);
            qry->bindValue(":number_of_rounds", number_of_rounds);
            qry->bindValue(":date", date);
            qry->bindValue(":manage_tournament", manage_tournament);
            if(qry->exec()){
                //ODŚWIERZANIE OKNA
                QMessageBox::information(this,tr("success"),tr("Turniej został edytowany"));
                this->close();
                addtournament *newDialog = new addtournament();
                newDialog->show();
            }else{
                QMessageBox::critical(this,tr("Błąd"),qry->lastError().text());
            }
            mydb.close();
            manage_tournament = -1; //Nie ma aktualnie zarządzanego turnieju
        }
    }
    else{
        QMessageBox::critical(this,tr("Błąd"),tr("Najpierw wybierz turniej do edycji"));
    }
}

void addtournament::on_delete_tournament_clicked()
{
    if(manage_tournament > -1){ //JEŻELI WYBRAŁEŚ TURNIEJ
        QSqlDatabase mydb = QSqlDatabase::addDatabase("QSQLITE");
        mydb.setDatabaseName("database.db");
        mydb.open();
        QSqlQuery* qry = new QSqlQuery(mydb);
        qry->prepare("DELETE FROM tournaments WHERE id = :manage_tournament");
        qry->bindValue(":manage_tournament", manage_tournament);

        if(qry->exec()){
            //ODŚWIERZ OKNO
            QMessageBox::information(this,tr("success"),tr("Turniej został usunięty"));
            this->close();
            addtournament *newDialog = new addtournament();
            newDialog->show();
        }else{
            QMessageBox::critical(this,tr("Błądr"),qry->lastError().text());
        }

        mydb.close();
        manage_tournament = -1; //Nie ma aktualnie zarządzanego turnieju
    }
    else{
        QMessageBox::critical(this,tr("Błąd"),tr("Najpierw wybierz turniej do usunięcia"));
    }
}

//FUNKCJA SPRAWDZAJĄCA DANE Z FORMULARZA
bool addtournament::checkTournamentInputs(QString arbiter, QString place, QString name, QString number_of_rounds){
    if(arbiter.size() ==0){
        QMessageBox::critical(this,tr("Błąd"),tr("Musisz podać sędziego turnieju!"));
        return false;
    }

    if(place.size() ==0){
        QMessageBox::critical(this,tr("Błąd"),tr("Musisz podać miejsce turnieju!"));
        return false;
    }

    if(number_of_rounds.size() ==0){
        QMessageBox::critical(this,tr("Błąd"),tr("Musisz podać liczbę rund"));
        return false;
    }
    return true;
}
