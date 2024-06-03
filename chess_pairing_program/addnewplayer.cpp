#include "addnewplayer.h"
#include "ui_addnewplayer.h"
#include "mainwindow.h"
#include <QMessageBox>

addnewplayer::addnewplayer(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::addnewplayer)
{
    ui->setupUi(this);
    this->setWindowTitle("Zarządzanie zawodnikami"); // Ustawienie nowego tytułu okna
    //WYPISZ ZAWDONIKÓW Z BAZY DANYCH
    QSqlDatabase mydb = QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName("database.db");
    QSqlQueryModel * modal = new QSqlQueryModel();
    QSqlQuery* qry = new QSqlQuery(mydb);
    mydb.open();

    qry->prepare("SELECT player_id, name, surname, category, fide_rating, date_of_birth from players where player_id != -1");
    qry->exec();
    modal->setQuery(*qry);
    ui->tableView->setModel(modal);
    mydb.close();
}

addnewplayer::~addnewplayer(){
    delete ui;
}

void addnewplayer::on_pushButton_clicked(){
    //POBRANIE DANYCH Z FORMULARZA
    QString name, surname, sex,date_of_birth, federation, fide_rating, rating, category;
    name=ui->txt_name->text();
    surname=ui->txt_surname->text();
    sex=ui->txt_sex->currentText();
    date_of_birth=ui->txt_date->text();
    federation=ui->txt_federation->text();
    fide_rating=ui->txt_fide->text();
    rating=ui->txt_rating->text();
    category=ui->txt_category->text();

    // SPRAWDZANIE POPRAWNOŚCI PODANYCH DANYCH
    if(checkPlayerInputs(category, federation, fide_rating, name, rating, surname)){
        QSqlDatabase mydb = QSqlDatabase::addDatabase("QSQLITE");
        mydb.setDatabaseName("database.db");
        mydb.open();
        QSqlQuery* qry = new QSqlQuery(mydb);
        qry->prepare("INSERT INTO players (name, surname, sex, date_of_birth, federation, fide_rating, ranking, category) "
                    "VALUES (:name, :surname, :sex, :date_of_birth, :federation, :fide_rating, :ranking, :category)");
        qry->bindValue(":name", name);
        qry->bindValue(":surname", surname);
        qry->bindValue(":sex", sex);
        qry->bindValue(":date_of_birth", date_of_birth);
        qry->bindValue(":federation", federation);
        qry->bindValue(":fide_rating", fide_rating);
        qry->bindValue(":ranking", rating);
        qry->bindValue(":category", category);


        if(qry->exec()){
            QMessageBox::information(this,tr("success"),tr("Zawodnik został dodany do bazy zawodników"));
            this->close();
            addnewplayer *newDialog = new addnewplayer();
            newDialog->show();
        }else{
            QMessageBox::critical(this,tr("Błąd"),qry->lastError().text());
        }
        mydb.close();
    }
}

void addnewplayer::on_pushButton_2_clicked(){
    this->close();
}

void addnewplayer::on_tableView_clicked(const QModelIndex &index){
    manage_player = index.sibling(index.row(), 0).data().toInt(); // id zawodnika (kliknięty wiersz)
    if(manage_player > -1) {
        QSqlDatabase mydb = QSqlDatabase::addDatabase("QSQLITE");
        mydb.setDatabaseName("database.db");

        // Sprawdzenie, czy baza danych została otwarta
        if (!mydb.open()) {
            QMessageBox::warning(this, tr("Błąd"), tr("Nie można otworzyć bazy danych: ") + mydb.lastError().text());
            return;
        }
        QSqlQuery qry;
        qry.prepare("SELECT * FROM players where player_id=:val");
        qry.bindValue(":val", manage_player);

        if (qry.exec()) {
            //USTAWIENIE PÓL FORMULARZA (wynik kwerendy)
            ui->txt_name->setText(qry.value(1).toString());
            ui->txt_surname->setText(qry.value(2).toString());
            ui->txt_sex->setCurrentText(qry.value(3).toString());
            QDate parsedData = QDate::fromString(qry.value(4).toString(), "dd.MM.yyyy");
            ui->txt_date->setDate(parsedData);
            ui->txt_federation->setText(qry.value(5).toString());
            ui->txt_fide->setText(qry.value(6).toString());
            ui->txt_rating->setText(qry.value(7).toString());
            ui->txt_category->setText(qry.value(8).toString());
        } else {
            QMessageBox::warning(this, tr("Błąd"), tr("Nie udało się wykonać zapytania: ") + qry.lastError().text());
        }
        mydb.close();
    }
}

void addnewplayer::on_edit_player_clicked()
{
    if(manage_player != -1){
        //POBIERANIE DANYCH Z FORMULARZA
        QString category, date, federation, fide, name, rating, sex, surname;
        category=ui->txt_category->text();
        date=ui->txt_date->text();
        federation=ui->txt_federation->text();
        fide=ui->txt_fide->text();
        name=ui->txt_name->text();
        rating=ui->txt_rating->text();
        sex=ui->txt_sex->currentText();
        surname=ui->txt_surname->text();
        //SPRAWDZANIE POPRAWNOŚCI PODANYCH DANYCH
        if(checkPlayerInputs(category, federation, fide, name, rating, surname)){
            QSqlDatabase mydb = QSqlDatabase::addDatabase("QSQLITE");
            mydb.setDatabaseName("database.db");

            mydb.open();
            QSqlQuery* qry = new QSqlQuery(mydb);
            qry->prepare("UPDATE players SET name = :name, surname = :surname, sex = :sex, date_of_birth = :date_of_birth, federation = :federation, fide_rating = :fide_rating, ranking = :ranking, category = :category WHERE player_id = :manage_player");
            qry->bindValue(":name", name);
            qry->bindValue(":surname", surname);
            qry->bindValue(":sex", sex);
            qry->bindValue(":date_of_birth", date);
            qry->bindValue(":federation", federation);
            qry->bindValue(":fide_rating", fide);
            qry->bindValue(":ranking", rating);
            qry->bindValue(":category", category);
            qry->bindValue(":manage_player", manage_player);

            if(qry->exec()){
                QMessageBox::information(this,tr("success"),tr("Profil zawodnika został edytowany"));
                this->close();
                addnewplayer *newDialog = new addnewplayer();
                newDialog->show();
            }else{
                QMessageBox::critical(this,tr("Błąd"),qry->lastError().text());
            }
            mydb.close();
            manage_player = -1;
        }
    }
    else{
        QMessageBox::critical(this,tr("Błąd"),tr("Najpierw wybierz gracza do edycji"));
    }
}


void addnewplayer::on_pushButton_3_clicked()
{
    //USUWANIE ZAWODNIKA
    if(manage_player > -1){
        QSqlDatabase mydb = QSqlDatabase::addDatabase("QSQLITE");
        mydb.setDatabaseName("database.db");

        mydb.open();
        QSqlQuery* qry = new QSqlQuery(mydb);
        qry->prepare("DELETE FROM players WHERE player_id = :manage_player");
        qry->bindValue(":manage_player", manage_player);

        if(qry->exec()){
            QMessageBox::information(this,tr("success"),tr("Zawodnik został usunięty"));
            this->close();
            addnewplayer *newDialog = new addnewplayer();
            newDialog->show();
        }else{
            QMessageBox::critical(this,tr("Błąd"),qry->lastError().text());
        }
        mydb.close();
        manage_player = -1; //Nie ma aktualnie zarządzanego gracza
    }
    else{
        QMessageBox::critical(this,tr("Błąd"),tr("Najpierw wybierz zawodnika do usunięcia"));
    }
}

//SPRAWDZENIE POPRAWNOŚCI DANYCH Z FORMULARZA
bool addnewplayer::checkPlayerInputs(QString category, QString federation, QString fide, QString name, QString rating, QString surname){
    //SPRAWDZENIE KATEGORII
    if(category.size() == 0){
        category = "bk";
    }
    static const QSet<QString> validCategory = {
        "bk", "V", "IV", "III", "II", "II+", "I", "I+", "I++",
        "k", "k+", "k++", "m", "CM", "FM", "IM", "GM",
        "WCM", "WFM", "WIM", "WGM"
    };
    if(!validCategory.contains(category)){
        QMessageBox::critical(this,tr("Błąd"),tr("Musisz podać poprawną kategorię szachową - jeżeli nie posiadasz, pozostaw puste"));
        return false;
    }
    //SPRAWDZENIE RANKINGU
    if(rating.size() == 0){
        rating = "1000";
    }
    static const QSet<QString> validRating = {
        "1000", "1100", "1200", "1250", "1400", "1600", "1700",
        "1800", "1900", "2000", "2100", "2200", "2300", "2400"
    };
    if(!validRating.contains(rating)){
        QMessageBox::critical(this,tr("Błąd"),tr("Musisz podać poprawny ranking - jeżeli nie posiadasz, pozostaw puste"));
        return false;
    }
    //SPRAWDZENIE SKRÓTU FEDERACJI
    if(federation.size() != 3){
        QMessageBox::critical(this,tr("Błąd"),tr("Podaj poprawny skrót federacji"));
        return false;
    }
    //SPRAWDZENIE RANKINGU FIDE
    bool ok;
    int value = fide.toInt(&ok);
    if (!ok || value < 1400 || value>3000){
        QMessageBox::critical(this,tr("Błąd"),tr("Ranking fide musi być liczbą w przedziale od 1400 do 3000"));
        return false;
    }
    //SPRAWDZENIE IMIENIA
    if(name.size() == 0){
        QMessageBox::critical(this,tr("Błąd"),tr("Musisz podać imię zawodnika!"));
        return false;
    }
    //SPRAWDZENIE NAZWISKA
    if(surname.size() == 0){
        QMessageBox::critical(this,tr("Błąd"),tr("Musisz podać nazwisko zawodnika!"));
        return false;
    }
    return true; // jeżeli wszystko poprawnie zwróci true
}
