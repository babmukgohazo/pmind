#ifndef PROPERTYTAB_H
#define PROPERTYTAB_H

class NodeLabel;
class NodeWidget;
#include <QDockWidget>
#include <QColorDialog>
#include <QString>
#include <QTextEdit>


namespace Ui {
class PropertyTab;
}

class PropertyTab : public QDockWidget
{
    Q_OBJECT
enum nodeShape{nothing,rec,underline,roundRec};
public:
    explicit PropertyTab(QWidget *parent = 0);
    ~PropertyTab(); 
    void setButtonImg(); //처음에 버튼의 이미지를 설정한다.
    void setDockWedigetDefault(); //dockWidget의 값을 초기화 한다.
    void setNodeWidget(NodeWidget * map){this->map = map;} //NodeWidget을 설정한다.

public slots:

 //ui에 있는 속성들이 각각 눌렸을 때
    void on_fontBox_currentFontChanged(const QFont &f); //폰트가 바뀌었으면 불러올 슬롯

    void on_buttonBold_clicked(); //굵기 버튼이 눌리면 처리할 슬롯

    void on_buttonItalic_clicked(); //기울임 버튼이 눌리면 처리할 슬롯

    void on_contentSizeBox_valueChanged(int arg1); //글씨 크기 사이즈가 바뀌면 처리할 슬롯

    void on_rectangleButton_clicked(); //테두리 네모버튼을 누르면 처리할 슬롯

    void on_roundRecButton_clicked(); //테두리 둥근네모를 누르면 처리할 슬롯

    void on_underlineButton_clicked(); //테두리 밑줄을 누르면 처리할 슬롯

    void on_buttonColor_clicked(); //글씨 색깔 버튼을 누르면 처리할 슬롯(다이얼로그를 띄운다.)

    void changeTextOfColor(); //글씨 색깔을 바꿈

    void showAllProperty(); //node가 가지고 있는 속성을 dockWidget에 표현한다.

    void propertyEnabled(); //Widget의 enable을 true로 한다.
    void propertyUnEnabled(); //Widget의 enable을 false로 한다.

private:
    Ui::PropertyTab *ui;
    NodeWidget * map;
    NodeLabel* focusedNode;
    QColorDialog *colorDial;
};

#endif // PROPERTYTAB_H
