#include "namecheck.h"
#include "ui_namecheck.h"
#include <QString>

nameCheck::nameCheck(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::nameCheck)
{
    ui->setupUi(this);
}

nameCheck::~nameCheck()
{
    delete ui;
}

void nameCheck::on_okButton_clicked()//이름 받는 바에서 ok버튼 눌렀을 때
{
    QString nameOfFile = ui->nameEdit->toPlainText();
    //이름 중복 확인하는 함수
}
