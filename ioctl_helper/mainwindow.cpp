#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->listOfDevices->addItem("HEllo");
    ui->listOfDevices->addItem("Nope");
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

