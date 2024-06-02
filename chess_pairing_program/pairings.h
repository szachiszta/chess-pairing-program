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
    explicit pairings(QWidget *parent = nullptr, QVector<QString> playerIds = {});
    ~pairings();

private:
    Ui::pairings *ui;
    QVector<QString> playerIds; // Tablica przechowująca ID zawodników
};

#endif // PAIRINGS_H
