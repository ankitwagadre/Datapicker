#include "mainwindow.h"
#include <QApplication>
#include <Datapicker.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();
    Datapicker dataobj;
    dataobj.showMaximized();
    dataobj.show();

    return a.exec();
}
