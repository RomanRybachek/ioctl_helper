#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QTableWidget>

void MainWindow::FillTableOfObjects(dir_obj_pair &names_of_dir_objects){

    ui->tableOfObjects->setRowCount(0);

    for(int i = 0; i < names_of_dir_objects.size() - 1; i++)
    {
        QString obj_name = QString::fromWCharArray(names_of_dir_objects[i].first.c_str());
        QString obj_type = QString::fromWCharArray(names_of_dir_objects[i].second.c_str());
        ui->tableOfObjects->insertRow(ui->tableOfObjects->rowCount());
        ui->tableOfObjects->setItem(ui->tableOfObjects->rowCount() - 1, 0, new QTableWidgetItem(obj_name));
        ui->tableOfObjects->setItem(ui->tableOfObjects->rowCount() - 1, 1, new QTableWidgetItem(obj_type));
        ui->tableOfObjects->item(ui->tableOfObjects->rowCount() - 1, 0)->setFlags(Qt::ItemIsEnabled);
        ui->tableOfObjects->item(ui->tableOfObjects->rowCount() - 1, 1)->setFlags(Qt::ItemIsEnabled);
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ioctl_hlpr = new ioctl_helper();
    this->dir_name = std::wstring(L"\\GLOBAL??");
    this->dir_objs = ioctl_hlpr->enum_directory_objects(this->dir_name);
    this->FillTableOfObjects(this->dir_objs);
}

MainWindow::~MainWindow()
{
    delete(ioctl_hlpr);
    delete ui;
}

void MainWindow::on_tableOfObjects_cellClicked(int row, int column)
{
    if (column == 0)
    {
        QString text = QString("\\\\.\\") + ui->tableOfObjects->item(row, column)->text();
        ui->curDeviceLineEdit->setText(text);
    }
}

void MainWindow::on_refreshDeviceButton_clicked()
{
    this->dir_name = std::wstring(L"\\GLOBAL??");
    this->dir_objs = ioctl_hlpr->enum_directory_objects(this->dir_name);
    this->FillTableOfObjects(this->dir_objs);
}

void MainWindow::on_findDeviceButton_clicked()
{
    ui->tableOfObjects->setRowCount(0);
//    QString pattern = ui->findDeviceButton->text();
//    dir_obj_pair found_objs;
//    dir_obj_pair &dir_objs = this->dir_objs;

}



