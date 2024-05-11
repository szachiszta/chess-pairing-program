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

private:
    Ui::addtournament *ui;
};

#endif // ADDTOURNAMENT_H
