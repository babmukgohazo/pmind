#include "headers/mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    a.setApplicationName(QString("PPoMIND"));
    a.setApplicationVersion(QString("1.0"));

    return a.exec();
}

