#include "propertytab.h"
#include "ui_propertytab.h"
#include "headers/mainwindow.h"
#include <QDebug>

PropertyTab::PropertyTab(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::PropertyTab)
{

    ui->setupUi(this);
    setButtonImg();

}

PropertyTab::~PropertyTab()
{
    delete ui;
}

void PropertyTab::keyPressEvent(QKeyEvent *e){
    mapScreen->setFocus();
    switch(e->key()){
    case Qt::Key_Tab:
    case Qt::Key_Enter:
    case Qt::Key_Return:
        QDockWidget::keyPressEvent(e);
        break;
    default:
        mapScreen->keyPressEvent(e);
        break;
    }
}

void PropertyTab::on_fontBox_currentFontChanged(const QFont &f)//글꼴
{
   NodeWidget* root= NodeWidget::searchFocusInNode(map);
   if(root==nullptr)
       return;
   if(root->isImageMode())
       return;
   focusedNode = root->labelPointer();//focused 된 라벨의 주소를 받아왔다 치자
   QFont font = ui->fontBox->currentFont();
   QFont fontOfNode = focusedNode->font();
   if(fontOfNode.bold()) font.setBold(true);
   if(fontOfNode.italic()) font.setItalic(true);
   font.setPointSize(fontOfNode.pointSize());
   focusedNode->setFont(font);
   root->setEditFont(font);
   emit fontChanged(root,fontOfNode);
   showAllProperty();
}

void PropertyTab::on_buttonBold_clicked()//굵기
{
     NodeWidget* root= NodeWidget::searchFocusInNode(map);
     if(root==nullptr)
         return;
     if(root->isImageMode())
         return;
     focusedNode = root->labelPointer(); //focused 된 라벨의 주소를 받아왔다 치자
     QFont font = focusedNode->font();
     QFont lastFont = font;
     if(font.bold())//만약 이미 굵은 상태
        font.setBold(false);
     else
         font.setBold(true);
     focusedNode->setFont(font);
     root->setEditFont(font);
     if(font!=lastFont)
         emit fontChanged(root,lastFont);
     showAllProperty();
}

void PropertyTab::on_buttonItalic_clicked()//기울임
{
    NodeWidget* root= NodeWidget::searchFocusInNode(map);
    if(root==nullptr)
        return;
    if(root->isImageMode())
        return;
    focusedNode = root->labelPointer(); //focused 된 라벨의 주소를 받아왔다 치자
    QFont font = focusedNode->font();
    QFont lastFont = font;
    if(font.italic())//만약 이미 기울어진 상태
       font.setItalic(false);
    else
        font.setItalic(true);
    focusedNode->setFont(font);
    root->setEditFont(font);
    if(font!=lastFont)
        emit fontChanged(root,lastFont);
    showAllProperty();
}

void PropertyTab::on_contentSizeBox_valueChanged(int arg1)//글씨크기
{
    NodeWidget* root= NodeWidget::searchFocusInNode(map);
    if(root==nullptr)
        return;
    if(root->isImageMode())
        return;
    focusedNode = root->labelPointer();
    QFont font = focusedNode->font();
    QFont lastFont = font;
    font.setPointSize(ui->contentSizeBox->value());
    focusedNode->setFont(font);
    root->setEditFont(font);
    if(font!=lastFont)
        emit fontChanged(root,lastFont);
    showAllProperty();
}



void PropertyTab::on_rectangleButton_clicked()//노드 모양 네모
{
    NodeWidget* root= NodeWidget::searchFocusInNode(map);
    if(root==nullptr)
        return;
    if(root==map)
        return;
    if(root->isImageMode())
        return;
    focusedNode = root->labelPointer();
    QString textColor = focusedNode->getNodeTextColor();
    if(focusedNode->getNodeShape()==rec){//만약에 애초에 네모였음
        focusedNode->setNodeShape(nothing);
    }else{//네모가 아니였음
        focusedNode->setNodeShape(rec);
    }
    QString shapeOfNode = focusedNode->getNodeShapeCSS();
    QString borderColor = focusedNode->getDefaultColorCSS();
    focusedNode->setStyleSheet(shapeOfNode+textColor+borderColor+"background-color : #6699ff;");
    showAllProperty();
    emit nodeStyleChanged(root, rec);
}

void PropertyTab::on_roundRecButton_clicked()//노드 모양 둥근 네모
{
    NodeWidget* root= NodeWidget::searchFocusInNode(map);
    if(root==nullptr)
        return;
    if(root==map)
        return;
    if(root->isImageMode())
        return;
    focusedNode = root->labelPointer();
    QString textColor = focusedNode->getNodeTextColor();
    if(focusedNode->getNodeShape()==roundRec){
        focusedNode->setNodeShape(nothing);
    }else{
        focusedNode->setNodeShape(roundRec);
    }
    QString shapeOfNode = focusedNode->getNodeShapeCSS();
    QString borderColor = focusedNode->getDefaultColorCSS();
    focusedNode->setStyleSheet(shapeOfNode+textColor+borderColor+"background-color : #6699ff;");
    showAllProperty();
    emit nodeStyleChanged(root, roundRec);
}


void PropertyTab::on_underlineButton_clicked()//노드 모양 밑줄
{
    NodeWidget* root= NodeWidget::searchFocusInNode(map);
    if(root==nullptr)
        return;
    if(root==map)
        return;
    if(root->isImageMode())
        return;
    focusedNode = root->labelPointer();
    QString textColor = focusedNode->getNodeTextColor();
    if(focusedNode->getNodeShape()==underline){
        focusedNode->setNodeShape(nothing);
    }else{
        focusedNode->setNodeShape(underline);
    }
    QString shapeOfNode = focusedNode->getNodeShapeCSS();
    QString borderColor = focusedNode->getDefaultColorCSS();
    focusedNode->setStyleSheet(shapeOfNode+textColor+borderColor+"background-color : #6699ff;");
    showAllProperty();
    emit nodeStyleChanged(root, underline);
}

void PropertyTab::on_buttonColor_clicked(){//글자색 입력받는 슬롯
    NodeWidget* root= NodeWidget::searchFocusInNode(map);
    if(root->isImageMode())
        return;
    colorDial = new QColorDialog();
    colorDial->show();
    QObject::connect(colorDial,SIGNAL(colorSelected(QColor)),this,SLOT(changeTextOfColor()));
}

void PropertyTab::changeTextOfColor(){//글자 색 바꾸는 슬롯
    NodeWidget* root= NodeWidget::searchFocusInNode(map);
    if(root==nullptr)
        return;
    focusedNode = root->labelPointer();

    focusedNode->setNodeTextColor(colorDial->selectedColor());
    QString textColor = focusedNode->getNodeTextColor();
    QString shape=focusedNode->getNodeShapeCSS();

    QString borderColor = focusedNode->getDefaultColorCSS();
    focusedNode->setStyleSheet(shape+textColor+borderColor+"background-color : #6699ff;");

    showAllProperty();
}

void PropertyTab::showAllProperty(){//node의 속성 dockWidget에 보여주기
    NodeWidget* root= NodeWidget::searchFocusInNode(map);
    if(root==nullptr)
        return;
    focusedNode = root->labelPointer();
    QFont font = focusedNode->font();
    setDockWedigetDefault();

    if(font.bold())//굵기
        ui->buttonBold->setStyleSheet("background-color : #0033cc;");


    if(font.italic())//기울임
        ui->buttonItalic->setStyleSheet("background-color : #0033cc;");

    ui->contentSizeBox->setValue(font.pointSize()); //글씨크기
    ui->fontBox->setCurrentText(font.toString()); //글꼴
    switch(focusedNode->getNodeShape()){
    case rec:
        ui->rectangleButton->setStyleSheet("background-color : #0033cc;"); break;
    case roundRec:
        ui->roundRecButton->setStyleSheet("background-color : #0033cc;"); break;
    case underline:
        ui->underlineButton->setStyleSheet("background-color : #0033cc;"); break;
    default :
        break;
    }
}
void PropertyTab::setDockWedigetDefault(){//dockWidget속성 초기화
    QFont notBoldFont;
    notBoldFont.setBold(false);
    ui->buttonBold->setFont(notBoldFont);
    ui->buttonItalic->setFont(notBoldFont);
    ui->buttonBold->setStyleSheet("background-color : QLinearGradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #79bbff, stop: 1 #378de5);");
    ui->buttonItalic->setStyleSheet("background-color : QLinearGradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #79bbff, stop: 1 #378de5);");
    ui->rectangleButton->setStyleSheet("background-color : QLinearGradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #79bbff, stop: 1 #378de5);");
    ui->roundRecButton->setStyleSheet("background-color : QLinearGradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #79bbff, stop: 1 #378de5);");
    ui->underlineButton->setStyleSheet("background-color : QLinearGradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #79bbff, stop: 1 #378de5);");
}

void PropertyTab::setButtonImg(){
    QPixmap colorCircle(":/buttonStyle/content/color_circle.png");
    QIcon colorButton(colorCircle);

    QPixmap boldImg(":/buttonStyle/content/bold_img.png");
    QIcon boldButton(boldImg);

    QPixmap italicImg(":/buttonStyle/content/italic_img.png");
    QIcon italicButton(italicImg);

    QPixmap rectangleImg(":/buttonStyle/node/rectangle_img.png");
    QIcon rectangleButton(rectangleImg);

    QPixmap roundRecImg(":/buttonStyle/node/roundRec_img.png");
    QIcon roundRecButton(roundRecImg);

    QPixmap underlineImg(":/buttonStyle/node/underline_img.png");
    QIcon underlineButton(underlineImg);

    QPixmap dottedLineImg(":/buttonStyle/line/dottedLine_img.png");
    QIcon dottedLineButton(dottedLineImg);

    ui->buttonColor->setIcon(colorButton);
    ui->buttonBold->setIcon(boldButton);
    ui->buttonItalic->setIcon(italicButton);
    ui->rectangleButton->setIcon(rectangleButton);
    ui->roundRecButton->setIcon(roundRecButton);
    ui->underlineButton->setIcon(underlineButton);
    ui->lineColorButton->setIcon(colorButton);
    ui->dottedLineButton->setIcon(dottedLineButton);
}

void PropertyTab::propertyEnabled(){
    this->setEnabled(true);
}

void PropertyTab::propertyUnEnabled(){
    this->setEnabled(false);
}
