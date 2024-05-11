#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QtDebug>
#include <QFileInfo>
#include "addplayertotournament.h"
#include "addnewplayer.h"
#include "tournamentslist.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    //MainWindow(QWidget *parent = nullptr);
    // W pliku źródłowym klasy MainWindow (mainwindow.cpp)
    explicit MainWindow(const QString &resultString = "",const QString &resultString2= "", QWidget *parent = nullptr);
    ~MainWindow();
    void setTournamentID(const QString &result);
    void setTournamentName(const QString &result);

private slots:
    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_btn_active_tournament_clicked();

private:
    Ui::MainWindow *ui;
    QString tournament_id = ""; //id aktywnego turnieju
    QString tournament_name = ""; // nazwa aktywnego turnieju
};
#endif // MAINWINDOW_H
