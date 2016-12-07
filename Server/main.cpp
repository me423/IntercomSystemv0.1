#include "mainwindow.h"
#include <QtCore>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;//("localhost", 8082);
    w.show();

    return a.exec();
}
