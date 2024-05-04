#ifndef ADDPLAYERTOTOURNAMENT_H
#define ADDPLAYERTOTOURNAMENT_H

#include <QDialog>

namespace Ui {
class addPlayerToTournament;
}

class addPlayerToTournament : public QDialog
{
    Q_OBJECT

public:
    explicit addPlayerToTournament(QWidget *parent = nullptr);
    ~addPlayerToTournament();

private:
    Ui::addPlayerToTournament *ui;
};

#endif // ADDPLAYERTOTOURNAMENT_H
