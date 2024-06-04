#ifndef PLAYERRESULTS_H
#define PLAYERRESULTS_H

#include <QDialog>
#include <QtSql>
#include <QMessageBox>

namespace Ui {
class PlayerResults;
}

class PlayerResults : public QDialog
{
    Q_OBJECT

public:
    explicit PlayerResults(QWidget *parent = nullptr, int PlayerID = -1, QString TournamentID = "-1");
    ~PlayerResults();

private:
    Ui::PlayerResults *ui;
    void ShowStatistics();
    int Player_id;
    QString Tournament_id;
};

#endif // PLAYERRESULTS_H
