#ifndef PROCESS_H
#define PROCESS_H

#include <QObject>
#include <QVector>
#include <QString>
#include <QFont>
#include <QColor>

class NodeWidget;
class Command;
enum nodeShape{nothing,rec,underline,roundRec,root};
enum class CommandType{
    Text, Add, Delete, Move, Font, NodeStyle
};

class Process : public QObject{
    Q_OBJECT
public:
    ~Process();

public slots:
    void push(Command*);
    void undo();
    void redo();

private:
    const int STACKSIZE = 50;
    QVector<Command*> undoStack;
    QVector<Command*> redoStack;
};

class Command{
public:
    Command(CommandType type_){this->type_=type_;}
    CommandType tpye(){return type_;}
    virtual void undo()=0;
    virtual void redo()=0;
    virtual NodeWidget* node(){return nullptr;}

protected:
    CommandType type_;
};

class TextCommand : public Command{
public:
    TextCommand(NodeWidget*);
    virtual void undo();
    virtual void redo();

private:
    NodeWidget* textChangedNode;
    QString lastText;
    QString text;

};

class AddCommand : public Command{//redo->delete
public:
    AddCommand(NodeWidget*);
    virtual void undo();
    virtual void redo();
    virtual NodeWidget* node(){return addedNode;}

private:
    NodeWidget* parent_;
    NodeWidget* addedNode;
    int index;
};

class DeleteCommand : public Command{//undo->delete
public:
    DeleteCommand(NodeWidget*);
    virtual void undo();
    virtual void redo();
    virtual NodeWidget* node(){return deletedNode;}

private:
    NodeWidget* parent_;
    NodeWidget* deletedNode;
    int index;
};

class MoveCommand : public Command{
public:
    MoveCommand(NodeWidget*, NodeWidget*);
    virtual void undo();
    virtual void redo();

private:
    NodeWidget* from;
    NodeWidget* to;
    NodeWidget* movedNode;
    int fromIndex;
    int toIndex;
    int fromColor;
    int toColor;
};

class FontCommand : public Command{
public:
    FontCommand(NodeWidget*, QFont);
    virtual void undo();
    virtual void redo();

private:
    NodeWidget* fontChangedNode;
    QFont lastFont;
    QFont font;
};

class NodeStyleCommand : public Command{
public:
    NodeStyleCommand(NodeWidget*, nodeShape);
    virtual void undo();
    virtual void redo();

private:
    NodeWidget* styleChangedNode;
    nodeShape shape;
};


#endif // PROCESS_H
