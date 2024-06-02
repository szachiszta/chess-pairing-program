#ifndef PAIRINGS_H
#define PAIRINGS_H

#include <QDialog>

namespace Ui {
class pairings;
}

class pairings : public QDialog
{
    Q_OBJECT

public:
    explicit pairings(QWidget *parent = nullptr, QVector<QString> playerIds = {}, QString tournament_id = "");
    ~pairings();

private slots:
    void on_pushButton_clicked();

private:
    Ui::pairings *ui;
    QVector<QString> playerIdsT; // Tablica przechowująca ID zawodników
    QString tournament_idT; //id aktywnego turnieju
    void generateRoundRobinPairings();
};

#endif // PAIRINGS_H
