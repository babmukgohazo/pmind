#ifndef PROPERTYTAB_H
#define PROPERTYTAB_H

#include <QDockWidget>
#include "headers/nodewidget.h"

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
    void showAllProperty(); //node가 가지고 있는 속성을 dockWidget에 표현한다.
    void setDockWedigetDefault(); //dockWidget의 값을 초기화 한다.
    void setNodeWidget(NodeWidget * map){this->map = map;}

private slots:
   // void on_fontComboBox_activated(const QString &arg1);

    void on_fontBox_currentFontChanged(const QFont &f);

    void on_buttonBold_clicked();

    void on_buttonItalic_clicked();

    void on_contentSizeBox_valueChanged(int arg1);

    void on_rectangleButton_clicked();

    void on_roundRecButton_clicked();

    void on_underlineButton_clicked();

private:
    Ui::PropertyTab *ui;
    NodeWidget * map;
    NodeLabel* focusedNode;
};

#endif // PROPERTYTAB_H
