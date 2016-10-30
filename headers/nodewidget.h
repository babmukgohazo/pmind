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

//트리 배치에 관련된 노드 구조
class NodeWidget : public QWidget{
public:
    NodeWidget(QString name = "Default");
    NodeWidget(QQueue<MdString> list);       //parsing된 mdstring 리스트를 이용해서 노드 생성 가능하도록 구현함.
    NodeWidget* getChild(int i);
    NodeWidget* getParent(){return parent_;}
    void add(NodeWidget *subNode);
    void paintEvent(QPaintEvent *e);

protected:
    QLabel selfWidget;
    QWidget childWidget;
    QHBoxLayout layout;
    QVBoxLayout childLayout;
    NodeWidget* parent_ = nullptr;
    QVector<NodeWidget*> child;
    int childNum = 0;
};


#endif // NODEWIDGET_H
