#ifndef PROCESS_H
#define PROCESS_H

#include <QObject>
#include <QVector>
#include <QString>
#include <QFont>
#include <QColor>
#include <QImage>

class NodeWidget;
class Command;
enum nodeShape{nothing,rec,underline,roundRec,root};
enum class CommandType{
    Text, Add, Delete, Move, Font, NodeStyle, Image
};

class Process : public QObject{//command의 집합이다. undo와 redo 기능을 관리하는 class이다.
    Q_OBJECT
public:
    ~Process();

public slots:
    void push(Command*);//redoStack 초기화 및 최대크기 제한한 push.
    void undo();//undoStack에서 top의 command를 반대로 실행한 후 pop한다.
    void redo();//redoStack에서 top의 command를 그대로 실행한 후 pop한다.

private:
    const int STACKSIZE = 50;//최대 undo의 size는 50이다.
    QVector<Command*> undoStack;//undo stack과 redo stack을 멤버로 가지고 있다.
    QVector<Command*> redoStack;
};

class Command{//command들의 base class이다
public:
    Command(CommandType type_){this->type_=type_;}
    CommandType tpye(){return type_;}
    virtual void undo()=0;//상속 받아서 command의 종류에 맞게 다시 정의해야 한다.
    virtual void redo()=0;
    virtual NodeWidget* node(){return nullptr;}

protected:
    CommandType type_;
};

class TextCommand : public Command{
public:
    TextCommand(NodeWidget*);//변경 전 마지막 text와 변경 후 text, 그리고 명령이 일어난 node의 주소를 설정
    virtual void undo();//text를 변경 전 마지막 text로 변경
    virtual void redo();//text를 변경 후의 text로 변경

private:
    NodeWidget* textChangedNode;//명령이 일어난 node
    QString lastText;//변경 전 마지막 text
    QString text;//변경 후 text

};

class AddCommand : public Command{//redo->delete
public:
    AddCommand(NodeWidget*);//명령이 일어난 node의 주소를 설정, node의 부모를 저장, node의 index를 저장
    virtual void undo();//명령이 일어난 node를 map으로부터 제거한다
    virtual void redo();//명령이 일어난 node를 원래 위치에 삽입한다
    virtual NodeWidget* node(){return addedNode;}
    //undo시 node가 delete되는 것이 아니라 command가 node를 가지고 있다가 redo시 다시 map에 넘겨주는 형식이다.

private:
    NodeWidget* parent_;//나중에 redo가 일어날 때 삽입할 노드를 기억해두기 위함
    NodeWidget* addedNode;//명령이 일어난 node
    int index;//나중에 redo가 일어날 때 삽입할 위치를 기억해두기 위함
};

class DeleteCommand : public Command{//undo->delete
public:
    DeleteCommand(NodeWidget*);//명령이 일어난 node의 주소를 설정, node의 부모를 저장, node의 index를 저장
    virtual void undo();//명령이 일어난 node를 원래 위치에 삽입한다
    virtual void redo();//명령이 일어난 node를 map으로부터 제거한다
    virtual NodeWidget* node(){return deletedNode;}

private:
    NodeWidget* parent_;//나중에 undo가 일어날 때 삽입할 노드를 기억해두기 위함
    NodeWidget* deletedNode;//명령이 일어난 node
    int index;//나중에 undo가 일어날 때 삽입할 위치를 기억해두기 위함
};

class MoveCommand : public Command{//add와 delete command를 모두 가지고 있다고 보면 된다. 조금 특별한 점은, root node에 연결하거나 떨어질 때 node의 색상이 바뀔 수 있어 이에 대한 정보도 저장한다는 것이다.
public:
    MoveCommand(NodeWidget*, NodeWidget*);//명령이 일어난 node의 주소를 설정, node의 부모를 저장, node의 index를 저장
    virtual void undo();//때어진 곳으로 node 이동, 자신 포함 모든 child 색상 fromColor로 설정
    virtual void redo();//놓인 곳으로 node 이동, 자신 포함 모든 child 색상 toColor로 설정

private:
    NodeWidget* from;//명령이 일어난 node의 parent
    NodeWidget* to;//drop된 node
    NodeWidget* movedNode;//명령이 일어난 node
    int fromIndex;//때어진 위치
    int toIndex;//놓인 위치
    int fromColor;//때어진 branch의 color
    int toColor;//놓여진 branch의 color
};

class FontCommand : public Command{
public:
    FontCommand(NodeWidget*, QFont);//명령이 일어난 node의 주소를 설정, 설정 전, 설정 후 font 저장
    virtual void undo();//설정 전 font로 변경
    virtual void redo();//설정 후 font로 변경

private:
    NodeWidget* fontChangedNode;//명령이 일어난 node
    QFont lastFont;//설정 전 font
    QFont font;//설정 후 font
};

class NodeStyleCommand : public Command{
public:
    NodeStyleCommand(NodeWidget*, nodeShape);//명령이 일어난 node의 주소를 설정, 변경된 shape의 종류를 저장
    virtual void undo();//변경된 shape를 다시 원래대로 돌림
    virtual void redo();//shape를 변경시킴

private:
    NodeWidget* styleChangedNode;//명령이 일어난 node
    nodeShape shape;//변경된 shape의 종류
};

class ImageCommand : public Command{
public:
    ImageCommand(NodeWidget*);//명령이 일어난 node의 주소를 설정, label의 image와 text를 저장해둠
    virtual void undo();//image상태에서 text상태로 변경
    virtual void redo();//text상태에서 image상태로 변경

private:
    NodeWidget* imageInsertedNode;//명령이 일어난 node
    QImage image;//label의 image
    QString text;//label의 text
};

#endif // PROCESS_H
