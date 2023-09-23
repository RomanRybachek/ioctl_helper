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
    dir_obj_pairs dir_objs;
    std::wstring dir_name;

private slots:
    void on_refreshDeviceButton_clicked();

    void on_findDeviceButton_clicked();

    void on_tableOfObjects_cellClicked(int row, int column);

    void on_findDeviceLineEdit_returnPressed();

    void on_CreateDevicePushButton_clicked();

    void on_openedDevicesTableWidget_cellClicked(int row, int column);

    void on_closeDevicePushButton_clicked();

private:
    Ui::MainWindow *ui;
    void FillTableOfObjects(dir_obj_pairs &names_of_dir_objects);
    void FillOpenedDevicesTable(opened_device_pairs &hanldes_and_names);
    void FindObject();
    void ErrorToOutput();
};
#endif // MAINWINDOW_H
