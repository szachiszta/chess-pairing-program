#ifndef ADDPLAYERTOTOURNAMENT_H
#define ADDPLAYERTOTOURNAMENT_H

#include <QDialog>

namespace Ui {
class addPlayerToTournament;
}

class addPlayerToTournament : public QDialog
{
    Q_OBJECT

signals:
    void playerAdded(QString playerId); // Sygnał emitowany po dodaniu zawodnika

public:
    explicit addPlayerToTournament(QWidget *parent = nullptr);
    ~addPlayerToTournament();

public slots:
    void on_tableView_activated(const QModelIndex &index);

private slots:
    void on_tableView_doubleClicked(const QModelIndex &index);

private:
    Ui::addPlayerToTournament *ui;
};

#endif // ADDPLAYERTOTOURNAMENT_H
