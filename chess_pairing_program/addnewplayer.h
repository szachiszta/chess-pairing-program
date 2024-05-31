#ifndef ADDNEWPLAYER_H
#define ADDNEWPLAYER_H

#include <QDialog>

namespace Ui {
class addnewplayer;
}

class addnewplayer : public QDialog
{
    Q_OBJECT

public:
    explicit addnewplayer(QWidget *parent = nullptr);
    ~addnewplayer();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_edit_player_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::addnewplayer *ui;
    int manage_player = -1;
};

#endif // ADDNEWPLAYER_H
