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

void PropertyTab::on_fontBox_currentFontChanged(const QFont &f)//글꼴
{

   NodeWidget* root= map->searchFocusInNode(map->getRoot());
   focusedNode = root->labelPointer();//focused 된 라벨의 주소를 받아왔다 치자
   QFont font = ui->fontBox->currentFont();
   QFont fontOfNode = focusedNode->font();
   if(fontOfNode.bold()) font.setBold(true);
   if(fontOfNode.italic()) font.setItalic(true);
   font.setPointSize(fontOfNode.pointSize());
   focusedNode->setFont(font);
   showAllProperty();
}

void PropertyTab::on_buttonBold_clicked()//굵기
{
     NodeWidget* root= map->searchFocusInNode(map->getRoot());
     focusedNode = root->labelPointer(); //focused 된 라벨의 주소를 받아왔다 치자
     QFont font = focusedNode->font();
     if(font.bold())//만약 이미 굵은 상태
        font.setBold(false);
     else
         font.setBold(true);
     focusedNode->setFont(font);
     showAllProperty();
}

void PropertyTab::on_buttonItalic_clicked()//기울임
{
    NodeWidget* root= map->searchFocusInNode(map->getRoot());
    focusedNode = root->labelPointer(); //focused 된 라벨의 주소를 받아왔다 치자
    QFont font = focusedNode->font();
    if(font.italic())//만약 이미 기울어진 상태
       font.setItalic(false);
    else
        font.setItalic(true);
    focusedNode->setFont(font);
     showAllProperty();
}

void PropertyTab::on_contentSizeBox_valueChanged(int arg1)//글씨크기
{
    NodeWidget* root= map->searchFocusInNode(map->getRoot());
    focusedNode = root->labelPointer();
    QFont font = focusedNode->font();
    font.setPointSize(ui->contentSizeBox->value());
    focusedNode->setFont(font);
    showAllProperty();
}



void PropertyTab::on_rectangleButton_clicked()//노드 모양 네모
{
    NodeWidget* root= map->searchFocusInNode(map->getRoot());
    focusedNode = root->labelPointer();
    if(focusedNode->getNodeShape()==rec){
        focusedNode->setStyleSheet("border-width: none");
        focusedNode->setNodeShape(nothing);
    }
    else{focusedNode->setStyleSheet("border-width: 2px;"
                               "border-style : solid;"
                               "border-color: black;");
        focusedNode->setNodeShape(rec);}
    showAllProperty();
}

void PropertyTab::on_roundRecButton_clicked()//노드 모양 둥근 네모
{
    NodeWidget* root= map->searchFocusInNode(map->getRoot());
    focusedNode = root->labelPointer();
    if(focusedNode->getNodeShape()==roundRec){
        focusedNode->setStyleSheet("border-width: none;");
        focusedNode->setNodeShape(nothing);
    }else{focusedNode->setStyleSheet("border-width: 2px;"
                               "border-style : solid;"
                               "border-radius: 4px;"
                               "border-color: black;");
    focusedNode->setNodeShape(roundRec);}
     showAllProperty();
}


void PropertyTab::on_underlineButton_clicked()//노드 모양 밑줄
{
    qDebug()<<"눌림";
    NodeWidget* root= map->searchFocusInNode(map->getRoot());
    focusedNode = root->labelPointer();
    if(focusedNode->getNodeShape()==underline){
        focusedNode->setStyleSheet("border-width: none;");
        focusedNode->setNodeShape(nothing);
    }else{focusedNode->setStyleSheet("border-top-style: none;"
                               "border-right-style: none;"
                               "border-bottom-style: solid;"
                               "border-left-style: none;"
                               "border-width: 2px;border-color: black;");
    focusedNode->setNodeShape(underline);}
    showAllProperty();
}

void PropertyTab::showAllProperty(){
    NodeWidget* root= map->searchFocusInNode(map->getRoot());
    focusedNode = root->labelPointer();
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
    default :
        break;
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
