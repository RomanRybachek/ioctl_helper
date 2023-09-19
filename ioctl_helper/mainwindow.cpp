#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QTableWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    std::wstring dir_name(L"\\Device");
    std::wstring dir_name(L"\\GLOBAL??");
    std::vector<std::pair<std::wstring, std::wstring>> names_of_dir_objects = ioctl_hlpr->enum_directory_objects(dir_name);

    for(int i = 0; i < names_of_dir_objects.size() - 1; i++)
    {
        QString obj_name = QString::fromWCharArray(names_of_dir_objects[i].first.c_str());
        QString obj_type = QString::fromWCharArray(names_of_dir_objects[i].second.c_str());
//        ui->listOfDevices->addItem(obj_name);
        ui->tableOfObjects->insertRow(ui->tableOfObjects->rowCount());
        ui->tableOfObjects->setItem(ui->tableOfObjects->rowCount() - 1, 0, new QTableWidgetItem(obj_name));
        ui->tableOfObjects->setItem(ui->tableOfObjects->rowCount() - 1, 1, new QTableWidgetItem(obj_type));
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


void MainWindow::on_tableOfObjects_cellClicked(int row, int column)
{
    if (column == 0)
    {
        QString text = QString("\\\\.\\") + ui->tableOfObjects->item(row, column)->text();
        ui->curDeviceLineEdit->setText(text);
    }
}

