#include "mainwindow.h"

#include <QApplication>


ioctl_helper *ioctl_hlpr;

int main(int argc, char *argv[])
{
    ioctl_hlpr = new ioctl_helper();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    delete(ioctl_hlpr);

    return a.exec();
}
