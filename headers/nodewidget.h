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
#include <QDrag>
#include <QMimeData>
#include <QDropEvent>
#include <QDebug>
#include "headers/parsing.h"
#include "headers/process.h"
#include "forms/propertytab.h"

class MainWindow;
class NodeWidget;
enum nodeShape{nothing,rec,underline,roundRec};
class NodeLabel : public QLabel{
    Q_OBJECT
public:
    NodeLabel(){
        setAcceptDrops(true);
        color="";
    }
    enum nodeShape{nothing,rec,underline,roundRec};
    void mousePressEvent(QMouseEvent *e);
    void keyPressEvent(QKeyEvent *e);
    void focusOutEvent(QFocusEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void dragEnterEvent(QDragEnterEvent *e);
    void dragLeaveEvent(QDragLeaveEvent *e);
    void dragMoveEvent(QDragMoveEvent *e);
    void dropEvent(QDropEvent *e);
    bool isFocus(){return focus;}

    void setNodeShape(int shape){nodeShape = shape;}
    int getNodeShape(){return nodeShape;}
    QString getNodeShapeCSS();
    void setNodeTextColor(QColor col){nodeTextColor=col.name();}
    QString getNodeTextColor(){return "color : "+nodeTextColor+";";}

    NodeWidget* container(){return container_;}
    void setContainer(NodeWidget* container_){this->container_=container_;}
    QColor& getColor(){return color;}

signals:
    void focused();
    void noFocused();
    void doubleClicked();
    void tabPressed();
    void enterPressed();
    void deletePressed();
    void keyPressed();
    void arrowPressed(int key);
    void redraw();
    void commanded(NodeWidget*, NodeWidget*, CommandType);

public slots:
    void focusIn();
    void focusOut();

private:
    bool focus = false;
    int nodeShape;
    QString nodeTextColor;
    NodeWidget* container_;
    QColor color;
    bool dragOver;
//<<<<<<< HEAD
    QPoint prePos;
//=======
    QString underlineCSS =  "border-top-style: none; border-right-style: none; border-bottom-style: solid; border-left-style: none; border-width: 4px;border-color: #ff6666;";
    QString recCSS = "border-width: 4px;border-style : solid;border-color: #ff6666;";
    QString roundRecCSS = "border-width: 4px; border-style : solid; border-radius: 5px; border-color: #ff6666;";
    QString nothingCSS = "border: 2px solid gray;";
//>>>>>>> feature/sprint_3_dockWidget
};

class NodeTextEdit : public QTextEdit{
    Q_OBJECT
public:
    void keyPressEvent(QKeyEvent *e);
    void focusOutEvent(QFocusEvent *e);
    QVector<QString>& textVector(){return textVector_;}
    QString labelText();
    void saveText(QString text_){this->text_ = text_;}
    QString getSavedText(){return text_;}

signals:
    void enterPressed();
    void focusOut();
    void escPressed();

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
    NodeLabel* labelPointer(){return &selfWidget;}
    int getIndex(){return index;}

    NodeWidget* takeNode();
    bool isChildOf(NodeWidget* ptr);

    void setEditFont(const QFont &);

    static NodeWidget* searchFocusInNode(NodeWidget* root);
    //2016/11/14일 추가한 함수

    static void setMainWindow(MainWindow* m){mainWindow = m;}

public slots:
    void labelToTextEdit();
    void textEditToLabel();
    void textEditSizeRenew();
    void labelSizeRenew();
    void makeDefaultChildNode();
    void makeDefaultSiblingNode();
    void deleteFromMap();
    void disconnectUpperNode();
    void focusMoveByArrow(int key);
    void closeTextEdit();

signals:
    void commanded(NodeWidget*, CommandType);
    void commanded(NodeWidget*, NodeWidget*, CommandType);

private:
    void init();

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

    static MainWindow* mainWindow;
    PropertyTab* dockWidget;
};

#endif // NODEWIDGET_H
