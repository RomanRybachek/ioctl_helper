#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QTableWidget>
#include <QCheckBox>

void MainWindow::FillTableOfObjects(dir_obj_pairs &names_of_dir_objects){

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

void MainWindow::FillOpenedDevicesTable(opened_device_pairs &hanldes_and_names){
    ui->openedDevicesTableWidget->setRowCount(0);
    for(int i = 0; i < hanldes_and_names.size(); i++){

        QString item_name;
        QString item_handle;

        std::stringstream stream;
        stream << std::hex << ioctl_hlpr->handles[i].second;
        item_handle += stream.str();
        item_name += ioctl_hlpr->handles[i].first.c_str();
//        ui->openedDevicesTableWidget->insertRow(item_handle);
        ui->openedDevicesTableWidget->insertRow(ui->openedDevicesTableWidget->rowCount());
        ui->openedDevicesTableWidget->setItem(ui->openedDevicesTableWidget->rowCount() - 1, 0, new QTableWidgetItem(item_handle));
        ui->openedDevicesTableWidget->setItem(ui->openedDevicesTableWidget->rowCount() - 1, 1, new QTableWidgetItem(item_name));
        ui->openedDevicesTableWidget->item(ui->openedDevicesTableWidget->rowCount() - 1, 0)->setFlags(Qt::ItemIsEnabled);
        ui->openedDevicesTableWidget->item(ui->openedDevicesTableWidget->rowCount() - 1, 1)->setFlags(Qt::ItemIsEnabled);
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
    ui->openedDevicesTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
//    ui->tableOfObjects->setColumnWidth(0, ui->tableOfObjects->width() / 3 * 2);
//    ui->tableOfObjects->setColumnWidth(1, ui->tableOfObjects->width() / 3);

    this->write_hex_to_display();
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
    dir_obj_pairs found_objs;
    dir_obj_pairs &dir_objs = this->dir_objs;

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
    DWORD 	dwDesiredAccess 	= 0;
    HANDLE 	handle				= NULL;
    std::wstring device_name 	= ui->curDeviceLineEdit->text().toStdWString();
    std::pair<std::wstring, HANDLE> new_device;
    if (device_name.length() == 0)
        return;

    if (ui->writeCheckBox->isChecked())
        dwDesiredAccess |= GENERIC_WRITE;
    if (ui->readCheckBox->isChecked())
        dwDesiredAccess |= GENERIC_READ;
    if (ui->execCheckBox->isChecked())
        dwDesiredAccess |= GENERIC_EXECUTE;

    handle = ioctl_hlpr->open_device(device_name, dwDesiredAccess);
    if ((long long)handle <= 0){
        this->ErrorToOutput();
    }
    else{
        this->FillOpenedDevicesTable(ioctl_hlpr->handles);
        std::stringstream stream;
        stream << std::hex << handle;
        QString text(stream.str().c_str());
        ui->curHandleLineEdit->setText(text);
    }
//    QString new_item = QString::fromWCharArray(new_device.first.c_str());
//    ui->openedDevicesListWidget->addItem(new_item);
//    CloseHandle(opened_device);
}

void MainWindow::ErrorToOutput(){
    std::string last_error = GetLastErrorAsString();
    QString text(last_error.c_str());
    ui->outputTextBrowser->append(text);
}

void MainWindow::on_openedDevicesTableWidget_cellClicked(int row, int column)
{
    QString name = ui->openedDevicesTableWidget->item(row, 1)->text();
    QString handle = ui->openedDevicesTableWidget->item(row, 0)->text();
    ui->curDeviceLineEdit->setText(name);
    ui->curHandleLineEdit->setText(handle);
}


void MainWindow::on_closeDevicePushButton_clicked()
{
    std::wstring dev_name = ui->curDeviceLineEdit->text().toStdWString();
    std::wstring dev_handle = ui->curHandleLineEdit->text().toStdWString();
    HANDLE handle;

    if (dev_handle.length() == 0)
        return;

    handle = (HANDLE)stoi(dev_handle, 0, 16);
    opened_device_pairs &handles = ioctl_hlpr->handles;

    for (opened_device_pairs::iterator i = handles.begin(); i != handles.end(); i++){
        if (i->first == dev_name and i->second == handle){
            QString text = QString::fromWCharArray(i->first.c_str());
            CloseHandle(i->second);
            handles.erase(i);
            this->FillOpenedDevicesTable(handles);
            return;
        }
    }
}

bytes_views MainWindow::parse_memory_with_chars(BYTE *mem, unsigned long long size){

    std::wstringstream stream;
    bytes_views bytes;

    for (unsigned long long i = 0; i < size; i++){
        stream << std::hex << mem[i];
        std::pair<std::wstring, std::wstring> one_byte;
        one_byte.first = stream.str();
        if (one_byte.first.length() == 1){
            one_byte.first = L"0" + one_byte.first;
        }
        if (std::isprint(mem[i]))
            one_byte.second = mem[i];
        else
            one_byte.second = '.';
        bytes.push_back(one_byte);
        stream.str(L"");
    }
    return bytes;
}

void MainWindow::write_hex_to_display(){

    BYTE *mem = (BYTE*)"aaaaaaaaaaaaaaaa";

    bytes_views mem_view = this->parse_memory_with_chars(mem, strlen((char*)mem) + 100);

    for (unsigned long long i = 0; i < mem_view.size(); i++){

        ui->hexOutputPlainTextEdit->insertPlainText(QString::fromWCharArray(mem_view[i].first.c_str()));
        QString hex_all = ui->hexOutputPlainTextEdit->toPlainText();
        int new_lines = hex_all.count(L"\n");
        if (hex_all.length() % (16 * 2 + 15 + new_lines) == 0){
            ui->hexOutputPlainTextEdit->insertPlainText(QString::fromWCharArray(L"\r"));
        }
        else
            ui->hexOutputPlainTextEdit->insertPlainText(QString::fromWCharArray(L" "));

        ui->charOutputPlainTextEdit->insertPlainText(QString::fromWCharArray(mem_view[i].second.c_str()));
        QString char_all = ui->charOutputPlainTextEdit->toPlainText();
        if (char_all.length() % 16 == 0){
            ui->hexOutputPlainTextEdit->insertPlainText(QString::fromWCharArray(L"\n"));
        }
    }
}
