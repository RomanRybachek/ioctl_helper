#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    std::wstring dir_name(L"\\Device");
    std::vector<std::wstring> names_of_dir_objects = ioctl_hlpr->enum_directory_objects(dir_name);

    for(int i = 0; i < names_of_dir_objects.size(); i++)
    {
        QString obj_name = QString::fromWCharArray(names_of_dir_objects[i].c_str());
        ui->listOfDevices->addItem(obj_name);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_listOfDevices_itemDoubleClicked(QListWidgetItem *item)
{
    ui->curDeviceLineEdit->setText(item->text());
}


void MainWindow::on_findDeviceButton_clicked()
{
}

