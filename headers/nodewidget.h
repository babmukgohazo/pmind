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
#include <QPainter>
#include "headers/parsing.h"
#include "headers/process.h"
#include "forms/propertytab.h"
#include "forms/textview.h"

class MainWindow;
class NodeWidget;
enum defaultColor{blue,red,green,orange,yellow,mint};

class NodeLabel : public QLabel{//node에서 text와 image를 저장하고 보여줄 widget
    Q_OBJECT
public:
    NodeLabel(){
        setAcceptDrops(true);
        color="";
    }
    enum nodeShape{nothing,rec,underline,roundRec,root};
    void mousePressEvent(QMouseEvent *e);//label이 click되었음을 mindmapView에게 알림.
    void mouseReleaseEvent(QMouseEvent *e);//click후 release되면 node가 focus됨. focus된 node였을 경우 textEdit으로 바뀜.
    void mouseDoubleClickEvent(QMouseEvent *e);//doubleClick이 일어나면 바로 textEdit으로 바뀜.
    void keyPressEvent(QKeyEvent *e);//label이 focus되었을 때 key가 입력되면 발생할 event. shortcut들이 정의됨.
    void focusOutEvent(QFocusEvent *e);//qt에서 직접 관리하는 focus기능. 잘 작동하지 않아 비활성화함.
    void mouseMoveEvent(QMouseEvent *e);//drag시작, drag 할 때 따라올 image를 설정.
    void dragEnterEvent(QDragEnterEvent *e);
    void dragLeaveEvent(QDragLeaveEvent *e);
    void dragMoveEvent(QDragMoveEvent *e);
    void dropEvent(QDropEvent *e);//끌어온 곳에서 연결을 끊고, drop하는 곳에 연결.
    bool isFocus(){return focus;}

    void setNodeShape(int shape){nodeShape = shape;}
    int getNodeShape(){return nodeShape;}
    QString getNodeShapeCSS();
    void setNodeTextColor(QColor col){nodeTextColor=col.name();}
    QString getNodeTextColor(){return "color : "+nodeTextColor+";";}
    void setDefaultColor(int a){
        defaultColor=a;
        QString shapeTmp =getNodeShapeCSS();//모양을 얻어온다
        QString colorTmp =getNodeTextColor();//글자 색을 얻어온다
        QString borderTmp =getDefaultColorCSS();//노드의 default 색깔 값을 얻어온다.
        setStyleSheet(shapeTmp+colorTmp+borderTmp); //바탕화면 파란색
    }
    int getDefaultColor(){return defaultColor;}
    QString getDefaultColorCSS();
    QString getDefaultColorString();

    NodeWidget* container(){return container_;}
    void setContainer(NodeWidget* container_){this->container_=container_;}
    QColor& getColor(){return color;}

signals:
    void focused();
    void noFocused();
    void doubleClicked();//label이 doubleClick되어 textEdit로 전환해야 한다는 신호.
    void tabPressed();//child node를 만들라는 신호
    void enterPressed();//sibling node를 만들라는 신호
    void deletePressed();//node를 map에서 제거하라는 신호
    void keyPressed();//문자나 숫자키가 입력되어 textEdit으로 전환해야 한다는 신호
    void arrowPressed(int key);//방향키가 입력되어 focus를 변경해야 한다는 신호
    void redraw();//update하라는 신호
    void commanded(NodeWidget*, NodeWidget*, CommandType);//node가 drop되어 undo stack에 drop command를 push해야 한다는 신호
    void labelClicked();//label이 click되었으므로 mindmapView를 focus하지 말라는 신호
    void italic();//italic으로 글꼴을 변경하라는 신호
    void bold();//bold로 글꼴을 변경하라는 신호
    void inImageMode();//image가 출력되는 상태이므로 propertyDock을 비활성화 하라는 신호
    void menuCall(const QPoint&);//우클릭되어 menu를 호출하라는 신호

public slots:
    void focusIn();//node가 focus되었을 때 작업할 일들. label색상 변경 등.
    void focusOut();//node가 focus 해제되었을 때 작업할 일들. label색상 변경 등.

private:
    bool focus = false;//node가 focus된 상태인지 저장
    int nodeShape;
    QString nodeTextColor;
    NodeWidget* container_;
    QColor color;
    bool dragOver;
    QPoint prePos;
    QString underlineCSS =  "border-top-style: none; border-right-style: none; border-bottom-style: solid; border-left-style: none; border-width: 2px;";
    QString recCSS = "border-width: 4px;border-style : solid;";
    QString roundRecCSS = "border-width: 4px; border-style : solid; border-radius: 5px;";
    QString nothingCSS = "border-top-style: none; border-right-style: none; border-bottom-style: solid; border-left-style: none; border-width: 2px;";
    QString rootCSS = "border-width: 3px; border-style : solid; border-color: #aed339;";
    int defaultColor;
    QString defaultColorCSS;
    QString defaultColorString;
};

class NodeTextEdit : public QTextEdit{
    Q_OBJECT
public:
    void keyPressEvent(QKeyEvent *e);//shortcut (줄바꿈, esc), enter시 label로 전환
    void focusOutEvent(QFocusEvent *e);//focus가 out될 때 edit에서 label로 전환
    void mousePressEvent(QMouseEvent *e);//edit의 focus상태를 mindmapView에게 전달.
    QVector<QString>& textVector(){return textVector_;}
    QString labelText();//가지고 있는 text string을 label형식으로 변환한다.
    void saveText(QString text_){this->text_ = text_;}
    QString getSavedText(){return text_;}

signals:
    void enterPressed();//enter가 눌리면 발생, label로 전환하라는 신호
    void focusOut();//edit 외부가 click되어 focusOut되면 발생, label로 전환하라는 신호
    void escPressed();//esc가 눌리면 발생, label로 전환하되, edit에서 작업한 변경사항은 적용하지 않음
    void editClicked();//edit가 click되면 발생, mindmapView가 focus되지 말라는 신호

private:
    QVector<QString> textVector_;//label에 출력될 text string을 만들 때 사용되는 공간이다. label의 text 한 줄 한 줄을 저장하고 있다. nodeWidget의 labelSizeRenew메서드 참고
    QString text_;//text를 저장할 공간. 마지막으로 edit가 label로 전환된 시점의 text를 가지고 있다. undo기능을 구현하기 위해 필요하다
    QFont font;//edit text의 font
};

class NodeWidget : public QWidget{
    Q_OBJECT
public:
    NodeWidget(QString name = "Default");//label에 설정할 text를 받아와 초기화.
    NodeWidget(QQueue<MdString> list, MainWindow* mainWindow);  //MdString list -> tree structure
    QVector<NodeWidget*>& getChild(){ return child; }
    NodeWidget* getParent(){return parent_;}
    NodeWidget* getRoot();//root node 반환
    int getDepth();
    int getDefaultColor();
    ~NodeWidget();

    void add(NodeWidget *subNode);//child vector 끝에 새로운 child를 추가한다.
    void insert(int index, NodeWidget *subNode);//특정 index에 child를 추가. child의 색상을 모두 parent와 같은 색으로 바꿈
    void onlyInsert(int index, NodeWidget *subNode);//색상을 바꾸지 않고 오직 삽입만 수행
    void paintEvent(QPaintEvent *e);//edge들을 그림. node의 우측 하단에서 시작하여 child의 좌측 하단으로 연결
    NodeTextEdit& getEdit(){return edit;}
    NodeLabel& label(){return selfWidget;}
    NodeLabel* labelPointer(){return &selfWidget;}
    QImage getImage(){return image;}
    int getIndex(){return index;}
    bool isEditMode(){return editMode;}
    bool isImageMode(){return imageMode;}
    NodeWidget* getNearestChild();//우측 방향키를 눌렀을 때 가장 가까운 child로 focus를 이동하기 위해 필요. 가장 가까운 child를 찾아서 반환

    NodeWidget* takeNode();
    bool isChildOf(NodeWidget* ptr);
    void setEditFont(const QFont &);

    static NodeWidget* searchFocusInNode(NodeWidget* root);//현재 focus되어있는 node를 찾아서 반환해준다
    static void setMainWindow(MainWindow* m){mainWindow = m;}
    static MainWindow* mainWindow;//mainwindow의 pointer에 쉽게 접근 가능하도록 저장

    QPen& getPen();
    void setPen(QPen& pen);

    static void setCounter(int c = 0){ counter = c;}

public slots:
    void labelToTextEdit();//label을 화면에서 지우고 textEdit를 보여준다
    void textEditToLabel();//textEdit를 화면에서 지우고 label을 보여준다
    void textEditSizeRenew();//textEdit에서 text가 바뀔 때 마다 호출되어 textEdit창의 크기를 조절한다.
    void labelSizeRenew();//textEdit상태에서 label로 바뀔 때 text를 label형태로 변환하면서 label의 크기를 조절한다
    void makeDefaultChildNode();
    void makeDefaultSiblingNode();
    void deleteFromMap();//할당해제가 아닌, map에서 보이지 않도록 widget을 close한다
    void disconnectUpperNode();//parent와의 연결을 끊는다
    void focusMoveByArrow(int key);//입력받은 방향키에 맞게 focus를 이동한다
    void closeTextEdit();//textEdit를 화면에서 지우고 label을 보여준다. 이때 label의 text는 textEdit에서 변경한 값이 적용되지 않는다
    void showCustomMenu(const QPoint &pos);//우클릭시 context menu를 보여준다
    void insertImage();//insert image버튼을 누를 때 호출되어 image를 불러온다
    void imageResize();//image를 정해진 규칙에 맞게 resize하고 label에 삽입한다

signals:
    void commanded(NodeWidget*, CommandType);//undo stack에 command를 추가하라는 신호이다.
    void commanded(NodeWidget*, NodeWidget*, CommandType);
    void generated();//노드가 생기거나 map에서 삭제될 때 호출되는 signal으로, propertyDock의 textEdit을 갱신하는데 사용된다.

private:
    void init();//초기화 메서드. 정의 참고

    QFont font;//text의 font를 저장
    QFontMetrics* fm;//font의 실제 화면 배치 size를 알아내기 위한 객체
    NodeLabel selfWidget;//label
    NodeTextEdit edit;//textEdit
    QWidget childWidget;//우측에 배치될 QVBoxLayout을 담을 widget
    QHBoxLayout layout;//좌측에 배치될 layout, label과 childWidget을 담음
    QVBoxLayout childLayout;//child들을 담을 layout
    NodeWidget* parent_ = nullptr;//parent
    QVector<NodeWidget*> child;//child node를 담은 vector
    static int counter;
    bool editMode = false;//textEdit가 출력되고 있는지
    bool imageMode = false;//label에 image가 출력되고 있는지
    int index = 0;//parent에서 볼 때의 위치
    QImage image;//label에서 출력될 image

    PropertyTab* PropertyDock;
    TextView* textViewDock;

    QPen pen;
};

#endif // NODEWIDGET_H
