#include "filemanage.h"
#include "namecheckdialog.h"
#include "ui_filemanage.h"
#include "ui_namecheckdialog.h"
#include "headers/mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include <QDir>

FileManage::FileManage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileManage)
{
    ui->setupUi(this);
}

FileManage::~FileManage()
{
    delete ui;
}

void FileManage::on_newButton_clicked() //new Button을 눌렀을 때
{
    emit signal_new();
    close();
}

void FileManage::on_openButton_clicked() //openButton을 눌렀을
{
    emit signal_open();
    close();
}

void FileManage::on_quitButton_clicked() //quit button을 눌렀을 때
{
    close();
}

//void FileManage::showMainWindow(QString f_fileName){ //MainWindow를 띄운다.
//  qDebug()<<"들어왔다";
//  QFile file(f_fileName);
//  if(!file.open(QIODevice::ReadOnly))
//    qDebug()<<"Error occured while opening the file";
//  MainWindow* workspace = new MainWindow(f_fileName);
//  workspace->show();
//  this->close();
//}
