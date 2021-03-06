#ifndef PROPERTYTAB_H
#define PROPERTYTAB_H

class NodeLabel;
class NodeWidget;
class MindmapView;
#include <QDockWidget>
#include <QColorDialog>
#include <QString>
#include <QTextEdit>
#include <QKeyEvent>


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
    void keyPressEvent(QKeyEvent *e);
    void setButtonImg();
    void setDockWedigetDefault(); //dockWidget의 값을 초기화 한다.
    void setNodeWidget(NodeWidget * map){this->map = map;}
    void setMapScreen(MindmapView* mapScreen){this->mapScreen = mapScreen;}
    QTextEdit* getTextEdit();

public slots:

 //ui에 있는 속성들이 각각 눌렸을 때
    void on_fontBox_currentFontChanged(const QFont &f);

    void on_buttonBold_clicked();

    void on_buttonItalic_clicked();

    void on_contentSizeBox_valueChanged(int arg1);

    void on_rectangleButton_clicked();

    void on_roundRecButton_clicked();

    void on_underlineButton_clicked();

    void on_buttonColor_clicked();

    void changeTextOfColor();

    void showAllProperty(); //node가 가지고 있는 속성을 dockWidget에 표현한다.

    void propertyEnabled();
    void propertyUnEnabled();

signals:
    void fontChanged(NodeWidget*, QFont);
    void nodeStyleChanged(NodeWidget*, nodeShape);

private slots:
    void on_fontBox_activated(const QString &arg1);

private:
    Ui::PropertyTab *ui;
    NodeWidget * map;
    NodeLabel* focusedNode;
    MindmapView* mapScreen;
    QColorDialog *colorDial;
};

#endif // PROPERTYTAB_H
