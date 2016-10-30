#include "filemanage.h"
#include "nameCheck.h"
#include "ui_filemanage.h"
#include "ui_namecheck.h"

fileManage::fileManage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::fileManage)
{
    ui->setupUi(this);
}

fileManage::~fileManage()
{
    delete ui;
}


void fileManage::on_newButton_clicked()// new 버튼 눌렀을 때
{
      nameCheck *a = new nameCheck(this);
      a->show();
}

void fileManage::on_openButton_clicked()//open button 눌렀을 때
{

}

void fileManage::on_quitButton_clicked()// quit button 눌렀을 때
{

}
