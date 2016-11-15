#include "propertytab.h"
#include "ui_propertytab.h"
#include "headers/nodewidget.h"
#include <QDebug>

PropertyTab::PropertyTab(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::PropertyTab)
{
    ui->setupUi(this);
}

PropertyTab::~PropertyTab()
{
    delete ui;
}

void PropertyTab::on_fontComboBox_currentFontChanged(const QFont &f)//글꼴
{
   // focusedNode= map->searchFocusInNode(map->getRoot());  //focused 된 라벨의 주소를 받아왔다 치자
  //  *focusedNode = map->getRoot()->label();
    focusedNode->setFont(ui->fontBox->currentFont());
}

void PropertyTab::on_buttonBold_clicked()//굵기
{
     NodeLabel* focusedNode = new NodeLabel();  //focused 된 라벨의 주소를 받아왔다 치자
     QFont font = focusedNode->font();
     if(font.bold())//만약 이미 굵은 상태
        font.setBold(true);
     else
         font.setBold(false);
     focusedNode->setFont(font);
}

void PropertyTab::on_buttonItalic_clicked()//기울임
{
    NodeLabel* focusedNode = new NodeLabel();  //focused 된 라벨의 주소를 받아왔다 치자
    QFont font = focusedNode->font();
    if(font.italic())//만약 이미 기울어진 상태
       font.setItalic(true);
    else
        font.setItalic(false);
    focusedNode->setFont(font);
}

void PropertyTab::on_contentSizeBox_valueChanged(int arg1)//글씨크기
{
    NodeLabel* focusedNode = new NodeLabel();  //focused 된 라벨의 주소를 받아왔다 치자
    QFont font = focusedNode->font();
    font.setPointSize(ui->contentSizeBox->value());
    focusedNode->setFont(font);
}



void PropertyTab::on_rectangleButton_clicked()//노드 모양 네모
{
    NodeLabel* focusedNode = new NodeLabel();
    focusedNode->setStyleSheet("border : solid 20px black;");
    focusedNode->setNodeShape(rec);
}

void PropertyTab::on_roundRecButton_clicked()//노드 모양 둥근 네모
{
    NodeLabel* focusedNode = new NodeLabel();
    focusedNode->setStyleSheet("border-width: 2px;"
                               "border-style : solid;"
                               "border-radius: 4px;"
                               "border-color: black;");
    focusedNode->setNodeShape(roundRec);
}

void PropertyTab::on_ovalButton_clicked()//노드 모양 타원 ***** painter사용 요망
{
    NodeLabel* focusedNode = new NodeLabel();
    focusedNode->setStyleSheet("border-width: 2px;"
                               "border-style : solid;"
                               "border-radius: 4px;"
                               "border-color: black;");
    focusedNode->setNodeShape(oval);
}

void PropertyTab::on_underlineButton_clicked()//노드 모양 밑줄
{
    NodeLabel* focusedNode = new NodeLabel();
    focusedNode->setStyleSheet("border-top-style: none;"
                               "border-right-style: none;"
                               "border-bottom-style: solid;"
                               "border-left-style: none;"
                               "border-width: 2px;border-color: black;");
    focusedNode->setNodeShape(underline);
}

void PropertyTab::showAllProperty(){
    NodeLabel* focusedNode = new NodeLabel();  //focused 된 라벨의 주소를 받아왔다 치자
    QFont font = focusedNode->font();
    QFont boldFont;
    boldFont.setBold(true);
    setDockWedigetDefault();

    if(font.bold())//굵기
        ui->buttonBold->setFont(boldFont);

    if(font.italic())//기울임
        ui->buttonItalic->setFont(boldFont);

    ui->contentSizeBox->setValue(font.pointSize()); //글씨크기
    ui->fontBox->setCurrentText(font.toString()); //글꼴
    switch(focusedNode->getNodeShape()){
    case rec:
        ui->rectangleButton->setFont(boldFont); break;
    case roundRec:
        ui->roundRecButton->setFont(boldFont); break;
    case underline:
        ui->underlineButton->setFont(boldFont); break;
    }
}
void PropertyTab::setDockWedigetDefault(){
    QFont notBoldFont;
    notBoldFont.setBold(false);
    ui->buttonBold->setFont(notBoldFont);
    ui->buttonItalic->setFont(notBoldFont);
    ui->rectangleButton->setFont(notBoldFont);
    ui->roundRecButton->setFont(notBoldFont);
    ui->underlineButton->setFont(notBoldFont);
}
