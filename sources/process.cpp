#include "headers/mainwindow.h"

Process::~Process(){
    while(!undoStack.isEmpty()){
        if(undoStack.front()->tpye() == CommandType::Delete)
            delete undoStack.front()->node();
        delete undoStack.front();
        undoStack.pop_front();
    }

    while(!redoStack.isEmpty()){
        if(redoStack.front()->tpye() == CommandType::Add)
            delete redoStack.front()->node();
        delete redoStack.front();
        redoStack.pop_front();
    }
}

void Process::push(Command* command){
    undoStack.push_back(command);

    if(undoStack.count() > STACKSIZE){
        if(undoStack.front()->tpye() == CommandType::Delete)
            delete undoStack.front()->node();
        delete undoStack.front();
        undoStack.pop_front();
    }

    while(!redoStack.isEmpty()){
        if(redoStack.front()->tpye() == CommandType::Add)
            delete redoStack.front()->node();
        delete redoStack.front();
        redoStack.pop_front();
    }
}

void Process::undo(){
    if(undoStack.count() == 0)
        return;

    undoStack.last()->undo();
    redoStack.push_back(undoStack.last());

    if(redoStack.count() > STACKSIZE){
        if(redoStack.front()->tpye() == CommandType::Add)
            delete redoStack.front()->node();
        delete redoStack.front();
        redoStack.pop_front();
    }
    undoStack.pop_back();
}

void Process::redo(){
    if(redoStack.count() == 0)
        return;

    redoStack.last()->redo();
    undoStack.push_back(redoStack.last());
    redoStack.pop_back();
}

TextCommand::TextCommand(NodeWidget* textChangedNode) : Command(CommandType::Text){
    this->textChangedNode = textChangedNode;
    lastText = textChangedNode->getEdit().getSavedText();
    text = textChangedNode->getEdit().toPlainText();
}

void TextCommand::undo(){
    textChangedNode->getEdit().saveText(lastText);
    textChangedNode->labelSizeRenew();
}

void TextCommand::redo(){
    textChangedNode->getEdit().saveText(text);
    textChangedNode->labelSizeRenew();
}

AddCommand::AddCommand(NodeWidget* addedNode) : Command(CommandType::Add){
    this->addedNode = addedNode;
    parent_=addedNode->getParent();
    index = addedNode->getIndex();
}

void AddCommand::undo(){
    addedNode->disconnectUpperNode();
    addedNode->close();
    addedNode->label().focusOut();
}

void AddCommand::redo(){
    parent_->insert(index, addedNode);
    addedNode->show();
}

DeleteCommand::DeleteCommand(NodeWidget* deletedNode) : Command(CommandType::Delete){
    this->deletedNode = deletedNode;
    parent_=deletedNode->getParent();
    index = deletedNode->getIndex();
}

void DeleteCommand::undo(){
    parent_->insert(index, deletedNode);
    deletedNode->show();
}

void DeleteCommand::redo(){
    deletedNode->disconnectUpperNode();
    deletedNode->close();
    deletedNode->label().focusOut();
}

MoveCommand::MoveCommand(NodeWidget *movedNode, NodeWidget *to) : Command(CommandType::Move){
    this->movedNode = movedNode;
    this->to = to;
    from = movedNode->getParent();
    fromIndex = movedNode->getIndex();
    toIndex = to->getChild().count();
}

void MoveCommand::undo(){
    movedNode->disconnectUpperNode();
    movedNode->close();
    from->insert(fromIndex, movedNode);
    movedNode->show();
}

void MoveCommand::redo(){
    movedNode->disconnectUpperNode();
    movedNode->close();
    to->insert(toIndex, movedNode);
    movedNode->show();
}

FontCommand::FontCommand(NodeWidget *fontChangedNode, QFont lastFont) : Command(CommandType::Font){
    this->fontChangedNode = fontChangedNode;
    this->lastFont = lastFont;
    font = fontChangedNode->label().font();
}

void FontCommand::undo(){
    fontChangedNode->label().setFont(lastFont);
    fontChangedNode->getEdit().setFont(lastFont);
}

void FontCommand::redo(){
    fontChangedNode->label().setFont(font);
    fontChangedNode->getEdit().setFont(font);
}
