#include "namecheckdialog.h"
#include "ui_namecheckdialog.h"
#include <QString>


NameCheckDialog::NameCheckDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NameCheckDialog)
{
    ui->setupUi(this);
}

NameCheckDialog::~NameCheckDialog()
{
    delete ui;
}

void NameCheckDialog::on_okButton_clicked() //ok button  눌렀을 때
{
        QString nameOfFile = ui->lineEdit->text()+".md";
        int i=0;
        QFile file(nameOfFile);
        QFile outfile;
        if(!file.open(QIODevice::ReadOnly))//파일이 없으면
            i=1;
        if(i)//파일이 없으면 파일을 새로 만들고 workspace를 연다
        {
         outfile.setFileName(nameOfFile);
         outfile.open(QIODevice::WriteOnly | QIODevice::Text);
         fileName=nameOfFile;
        }
        else
        {//파일이 있으면 에러 메시지 띄우고 그대로 이름을 다시 받는다
            QMessageBox messageBox;
            messageBox.critical(0,"Error","이미 있는 파일입니다.");
        }
        file.close();
        outfile.close();
        FileManage* tmp = new FileManage();
//        tmp->showMainWindow(fileName);
        emit okButtonClicked();
}

void NameCheckDialog::on_cancelButton_clicked()//Cancel Button 눌렀을 때
{
    close();
}

