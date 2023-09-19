#include "mainwindow.h"

#include <QApplication>


ioctl_helper *ioctl_hlpr;

int main(int argc, char *argv[])
{


    QApplication a(argc, argv);
    MainWindow w;
    w.show();


    return a.exec();
}
