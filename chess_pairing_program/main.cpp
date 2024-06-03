#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    QString iconPath = "logo.jpg";
    // Ustawienie ikony dla aplikacji
    a.setWindowIcon(QIcon(iconPath));
    return a.exec();
}
