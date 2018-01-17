#include "mainwindow.h"
#include <QApplication>
#include <QMetaType>
#include <string>

#include "vld.h"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<string>("string");
    MainWindow w;
    w.show();

    return a.exec();
}
