#ifndef TOURNAMENTSLIST_H
#define TOURNAMENTSLIST_H

#include <QDialog>

namespace Ui {
class tournamentsList;
}

class tournamentsList : public QDialog
{
    Q_OBJECT

public:
    explicit tournamentsList(QWidget *parent = nullptr);
    ~tournamentsList();

private slots:
    void on_tableView_activated(const QModelIndex &index);

private:
    Ui::tournamentsList *ui;
};

#endif // TOURNAMENTSLIST_H
