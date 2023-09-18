#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include "ioctl_helper.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

extern ioctl_helper *ioctl_hlpr;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_listOfDevices_itemDoubleClicked(QListWidgetItem *item);

    void on_findDeviceButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
