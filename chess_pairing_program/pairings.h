#ifndef PAIRINGS_H
#define PAIRINGS_H

#include <QDialog>
#include "playerresults.h"

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
    void Show_pairings_round(int round);
    void on_current_round_currentIndexChanged(int index);
    void on_pairings_round_clicked(const QModelIndex &index);
    void Add_result(QString result);
    void Update_pairings_results();
    void on_b1_0_clicked(); // PRZYCISKI MOŻLIWYCH WYNIKÓW
    void on_b_0_0_clicked();
    void on_b_m_p_clicked();
    void on_b_p_m_clicked();
    void on_b_0_1_clicked();
    void on_b_1_2_clicked();
    void Update_results_table(QString condition = ""); //ODŚWIERZ TABELĘ

    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();

    void on_results_table_clicked(const QModelIndex &index);

private:
    Ui::pairings *ui;
    QVector<QString> playerIdsT; // Tablica przechowująca ID zawodników
    QString tournament_idT; //id aktywnego turnieju
    void generateRoundRobinPairings();
    int selected_white = -2;
    int selected_black = -2;
};

#endif // PAIRINGS_H
