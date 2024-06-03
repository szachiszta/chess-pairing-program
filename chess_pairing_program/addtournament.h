#ifndef ADDTOURNAMENT_H
#define ADDTOURNAMENT_H

#include <QDialog>

namespace Ui {
class addtournament;
}

class addtournament : public QDialog
{
    Q_OBJECT

public:
    explicit addtournament(QWidget *parent = nullptr);
    ~addtournament();

private slots:
    void on_pushButton_2_clicked();

    void on_add_tournament_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_edit_tournament_clicked();

    void on_delete_tournament_clicked();

    bool checkTournamentInputs(QString arbiter, QString place, QString name, QString number_of_rounds);

private:
    Ui::addtournament *ui;
    int manage_tournament = -1;
};

#endif // ADDTOURNAMENT_H
