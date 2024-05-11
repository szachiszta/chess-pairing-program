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

private:
    Ui::addnewplayer *ui;
};

#endif // ADDNEWPLAYER_H
