#ifndef NODEWIDGET_H
#define NODEWIDGET_H
#include <QVector>
#include <QPaintEvent>
#include <QPainter>
#include <QWidget>
#include <QLabel>
#include <QLayout>
#include <QQueue>
#include "headers/parsing.h"


class NodeWidget : public QWidget{
public:
    NodeWidget(QString name = "Default");
    NodeWidget(QQueue<MdString> list);  //MdString list -> tree structure
    QVector<NodeWidget*>& getChild(){ return child; }
    NodeWidget* getParent(){return parent_;}
    //int getChildNum(childNum)

    void add(NodeWidget *subNode);
    void paintEvent(QPaintEvent *e);

private:
    void init(){
        this->setStyleSheet("background-color: transparent");
        //selfWidget.setStyleSheet("background-color: transparent ; border-bottom: 1px solid black;");
        selfWidget.setStyleSheet("border: 2px solid gray;");
        //selfWidget.setSizePolicy(QSizePolicy::QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
        layout.addWidget(&selfWidget);
        layout.addWidget(&childWidget);
        layout.setSpacing(30);
        this->setLayout(&layout);
        childWidget.setLayout(&childLayout);
        layout.setContentsMargins(0,0,0,0);
        childLayout.setMargin(0);
    }

    QLabel selfWidget;
    QWidget childWidget;
    QHBoxLayout layout;
    QVBoxLayout childLayout;
    NodeWidget* parent_ = nullptr;
    QVector<NodeWidget*> child;
    int childNum = 0;
};


#endif // NODEWIDGET_H
