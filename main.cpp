#include "MainWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;

    w.setWindowIcon(QIcon(":/Resource/ico/N.ico"));

    w.show();
    return a.exec();
}
