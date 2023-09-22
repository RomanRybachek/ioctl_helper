#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QTableWidget>
#include <QCheckBox>

void MainWindow::FillTableOfObjects(dir_obj_pair &names_of_dir_objects){

    ui->tableOfObjects->setRowCount(0);

    for(int i = 0; i < names_of_dir_objects.size(); i++)
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
    ui->tableOfObjects->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
//    ui->tableOfObjects->setColumnWidth(0, ui->tableOfObjects->width() / 3 * 2);
//    ui->tableOfObjects->setColumnWidth(1, ui->tableOfObjects->width() / 3);
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

void MainWindow::FindObject(){

    std::wstring pattern = ui->findDeviceLineEdit->text().toStdWString();
    std::transform(pattern.begin(), pattern.end(), pattern.begin(), ::tolower);
    dir_obj_pair found_objs;
    dir_obj_pair &dir_objs = this->dir_objs;

    for (int i = 0; i < dir_objs.size(); i++)
    {
        std::wstring cur = dir_objs[i].first;
        std::transform(cur.begin(), cur.end(), cur.begin(), ::tolower);
        if (cur.find(pattern) != std::wstring::npos){
            found_objs.push_back(dir_objs[i]);
        }
    }

    this->FillTableOfObjects(found_objs);
}

void MainWindow::on_findDeviceButton_clicked()
{
    this->FindObject();
}

void MainWindow::on_findDeviceLineEdit_returnPressed()
{
    this->FindObject();
}


void MainWindow::on_CreateDevicePushButton_clicked()
{
    DWORD dwDesiredAccess = 0;

    if (ui->writeCheckBox->isChecked())
        ui->dbgLineEdit->setText(QString("w"));
    if (ui->readCheckBox->isChecked())
        ui->dbgLineEdit->setText(QString("r"));
    if (ui->execCheckBox->isChecked())
        ui->dbgLineEdit->setText(QString("e"));
//    ui->readCheckBox
//    ui->execCheckBox
//    CreateFileW(device_name.c_str(), )
}

