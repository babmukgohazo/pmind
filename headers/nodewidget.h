#ifndef NODEWIDGET_H
#define NODEWIDGET_H
#include <QApplication>
#include <QVector>
#include <QPaintEvent>
#include <QPainter>
#include <QWidget>
#include <QLabel>
#include <QLayout>
#include <QQueue>
#include <QLineEdit>
#include <QTextEdit>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QFont>
#include <QScrollBar>
#include <QFocusEvent>
#include <QDebug>
#include "headers/parsing.h"

class MainWindow;

class NodeLabel : public QLabel{
    Q_OBJECT
public:
    void mousePressEvent(QMouseEvent *e);
    void keyPressEvent(QKeyEvent *e);
    void focusOutEvent(QFocusEvent *e);

signals:
    void doubleClicked();
    void tabPressed();
    void enterPressed();
    void deletePressed();
    void keyPressed();
    void arrowPressed(int key);
    void redraw();

public slots:
    void focusIn();
    void focusOut();

private:
    bool isFocus = false;
};

class NodeTextEdit : public QTextEdit{
    Q_OBJECT
public:
    void keyPressEvent(QKeyEvent *e);
    void focusOutEvent(QFocusEvent *e);
    QString text();
    QVector<QString>& textVector(){return textVector_;}
    QString labelText();
    void saveText(QString text_){this->text_ = text_;}
    QString getSavedText(){return text_;}

signals:
    void enterPressed();
    void focusOut();

private:
    QVector<QString> textVector_;
    QString text_;
};

class NodeWidget : public QWidget{
    Q_OBJECT
public:
    NodeWidget(QString name = "Default");
    NodeWidget(QQueue<MdString> list, MainWindow* mainWindow);  //MdString list -> tree structure
    QVector<NodeWidget*>& getChild(){ return child; }
    NodeWidget* getParent(){return parent_;}
    NodeWidget* getRoot();
    //int getChildNum(childNum)
    ~NodeWidget();

    void add(NodeWidget *subNode);
    void insert(int index, NodeWidget *subNode);
    void paintEvent(QPaintEvent *e);
    NodeTextEdit& getEdit(){return edit;}
    NodeLabel& label(){return selfWidget;}

public slots:
    void labelToTextEdit();
    void textEditToLabel();
    void textEditSizeRenew();
    void makeDefaultChildNode();
    void makeDefaultSiblingNode();
    void deleteThisNode();
    void focusMoveByArrow(int key);

private:
    void init(){
        fm = new QFontMetrics(edit.currentFont());
        this->setStyleSheet("background-color: transparent");
        //selfWidget.setStyleSheet("background-color: transparent ; border-bottom: 1px solid black;");
        selfWidget.setStyleSheet("border: 2px solid gray;");
        selfWidget.setSizePolicy(QSizePolicy::QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
        layout.addWidget(&selfWidget);
        layout.addWidget(&childWidget);
        layout.setSpacing(30);
        this->setLayout(&layout);
        childWidget.setLayout(&childLayout);
        layout.setContentsMargins(0,0,0,0);
        childLayout.setMargin(0);
        edit.verticalScrollBar()->close();
        QObject::connect(&edit,SIGNAL(enterPressed()),this,SLOT(textEditToLabel()));
        QObject::connect(&edit,SIGNAL(focusOut()),this,SLOT(textEditToLabel()));
        QObject::connect(&edit,SIGNAL(focusOut()),&selfWidget,SLOT(focusOut()));
        QObject::connect(&selfWidget,SIGNAL(doubleClicked()),this,SLOT(labelToTextEdit()));
        QObject::connect(&edit,SIGNAL(textChanged()),this,SLOT(textEditSizeRenew()));
        QObject::connect(&selfWidget,SIGNAL(tabPressed()),this,SLOT(makeDefaultChildNode()));
        QObject::connect(&selfWidget,SIGNAL(enterPressed()),this,SLOT(makeDefaultSiblingNode()));
        QObject::connect(&selfWidget,SIGNAL(deletePressed()),this,SLOT(deleteThisNode()));
        QObject::connect(&selfWidget,SIGNAL(keyPressed()),this,SLOT(labelToTextEdit()));
        QObject::connect(&selfWidget,SIGNAL(arrowPressed(int)),this,SLOT(focusMoveByArrow(int)));
        QObject::connect(&selfWidget,SIGNAL(redraw()),getRoot(),SLOT(update()));
    }

    QFont font;
    QFontMetrics* fm;
    NodeLabel selfWidget;
    NodeTextEdit edit;
    QWidget childWidget;
    QHBoxLayout layout;
    QVBoxLayout childLayout;
    NodeWidget* parent_ = nullptr;
    QVector<NodeWidget*> child;
    bool editMode = false;
    bool clicked = false;
    int index = 0;
    MainWindow* mainWindow;
};


#endif // NODEWIDGET_H
