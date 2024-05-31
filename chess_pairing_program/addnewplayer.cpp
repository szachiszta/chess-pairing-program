#include "addnewplayer.h"
#include "ui_addnewplayer.h"
#include "mainwindow.h"
#include <QMessageBox>

addnewplayer::addnewplayer(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::addnewplayer)
{
    ui->setupUi(this);
    QSqlDatabase mydb = QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName("database.db");

    QSqlQueryModel * modal = new QSqlQueryModel();

    QSqlQuery* qry = new QSqlQuery(mydb);
    mydb.open();

    qry->prepare("SELECT * from players");
    qry->exec();
    modal->setQuery(*qry);
    ui->tableView->setModel(modal);
    qDebug() << (modal->rowCount());
    mydb.close();
    mydb.removeDatabase("database.db");
}

addnewplayer::~addnewplayer()
{
    delete ui;
}

void addnewplayer::on_pushButton_clicked()
{
    QString name, surname, sex,date_of_birth, federation, fide_rating, rating, category;

    name=ui->txt_name->text();
    surname=ui->txt_surname->text();
    sex=ui->txt_sex->text();
    date_of_birth=ui->txt_date->text();
    federation=ui->txt_federation->text();
    fide_rating=ui->txt_fide->text();
    rating=ui->txt_rating->text();
    category=ui->txt_category->text();

    // TO DO SPRAWDZANIE POPRAWNOŚCI PODANYCH DANYCH

    QSqlDatabase mydb = QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName("database.db");

    mydb.open();
    QSqlQuery* qry = new QSqlQuery(mydb);
    qry->prepare("INSERT into players(name,surname,sex,date_of_birth,federation,fide_rating,ranking,category) values('"+name+"', '"+surname+"', '"+sex+"' , '"+date_of_birth+"' , '"+federation+"' , '"+fide_rating+"', '"+rating+"', '"+category+"')");

    if(qry->exec()){
        QMessageBox::information(this,tr("success"),tr("Zawodnik został dodany do bazy zawodników"));
    }else{
        QMessageBox::critical(this,tr("error"),qry->lastError().text());
    }



    mydb.close();
}


void addnewplayer::on_pushButton_2_clicked()
{
    this->close();
}


void addnewplayer::on_tableView_clicked(const QModelIndex &index)
{
    // Pobierz wartość z pierwszej kolumny klikniętego wiersza jako int
    manage_player = index.sibling(index.row(), 0).data().toInt();

    qDebug() << index.row();
    if(manage_player > -1) {
        QSqlDatabase mydb = QSqlDatabase::addDatabase("QSQLITE");
        mydb.setDatabaseName("database.db");

        // Sprawdzenie, czy baza danych została otwarta
        if (!mydb.open()) {
            QMessageBox::warning(this, tr("Błąd"), tr("Nie można otworzyć bazy danych: ") + mydb.lastError().text());
            return;  // Wyjście z funkcji, jeśli nie udało się otworzyć bazy danych
        }

        QSqlQuery qry;
        qry.prepare("SELECT * FROM players where player_id=:val");
        qry.bindValue(":val", manage_player);  // Bezpieczne zapytanie

        if (qry.exec()) {
            QString resultString;
            if (qry.next()) {
                resultString = qry.value(0).toString();  // ID zawodnika
            }
            ui->txt_category->setText(qry.value(8).toString()); // Ustawienie tekstu w polu txt_name
            ui->txt_date->setText(qry.value(4).toString());
            ui->txt_federation->setText(qry.value(5).toString());
            ui->txt_fide->setText(qry.value(6).toString());
            ui->txt_rating->setText(qry.value(7).toString());
            ui->txt_sex->setText(qry.value(3).toString());
            ui->txt_surname->setText(qry.value(2).toString());
            ui->txt_name->setText(qry.value(1).toString());
        } else {
            QMessageBox::warning(this, tr("Błąd"), tr("Nie udało się wykonać zapytania: ") + qry.lastError().text());
        }
        mydb.close();
    }
}


void addnewplayer::on_edit_player_clicked()
{
    if(manage_player != -1){
        QString category, date, federation, fide, name, rating, sex, surname;
        category=ui->txt_category->text();
        date=ui->txt_date->text();
        federation=ui->txt_federation->text();
        fide=ui->txt_fide->text();
        name=ui->txt_name->text();
        rating=ui->txt_rating->text();
        sex=ui->txt_sex->text();
        surname=ui->txt_surname->text();
        // TO DO SPRAWDZANIE POPRAWNOŚCI PODANYCH DANYCH

        QSqlDatabase mydb = QSqlDatabase::addDatabase("QSQLITE");
        mydb.setDatabaseName("database.db");

        mydb.open();
        QSqlQuery* qry = new QSqlQuery(mydb);
        qry->prepare("UPDATE players SET name = :name, surname = :surname, sex = :sex, date_of_birth = :date_of_birth, federation = :federation, fide_rating = :fide_rating, ranking = :ranking, category = :catergory WHERE player_id = :manage_player");
        qry->bindValue(":name", name);
        qry->bindValue(":surname", surname);
        qry->bindValue(":sex", sex);
        qry->bindValue(":date_of_birth", date);
        qry->bindValue(":federation", federation);
        qry->bindValue(":fide_rating", fide);
        qry->bindValue(":ranking", rating);
        qry->bindValue(":category", category);

        if(qry->exec()){
            QMessageBox::information(this,tr("success"),tr("Profil zawodnika został edytowany"));
            this->close();
        }else{
            QMessageBox::critical(this,tr("error"),qry->lastError().text());
        }
        mydb.close();
        manage_player = -1;
    }
    else{
        QMessageBox::critical(this,tr("error"),tr("Najpierw wybierz gracza do edycji"));
    }
}


void addnewplayer::on_pushButton_3_clicked()
{
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
        }else{
            QMessageBox::critical(this,tr("error"),qry->lastError().text());
        }

        mydb.close();
        manage_player = -1; //Nie ma aktualnie zarządzanego turnieju
    }
    else{
        QMessageBox::critical(this,tr("error"),tr("Najpierw wybierz zawodnika do usunięcia"));
    }
}

