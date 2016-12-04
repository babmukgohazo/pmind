#include "headers/mainwindow.h"
#include "headers/nodewidget.h"

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

    NodeWidget::mainWindow->update();
}

void Process::redo(){
    if(redoStack.count() == 0)
        return;

    redoStack.last()->redo();
    undoStack.push_back(redoStack.last());
    redoStack.pop_back();

    NodeWidget::mainWindow->update();
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
    parent_->onlyInsert(index, addedNode);
    addedNode->show();
}

DeleteCommand::DeleteCommand(NodeWidget* deletedNode) : Command(CommandType::Delete){
    this->deletedNode = deletedNode;
    parent_=deletedNode->getParent();
    index = deletedNode->getIndex();
}

void DeleteCommand::undo(){
    parent_->onlyInsert(index, deletedNode);
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
    fromColor = movedNode->label().getDefaultColor();
}

void MoveCommand::undo(){
    if(to==NodeWidget::mainWindow->getMap())
        toColor = movedNode->label().getDefaultColor();

    movedNode->disconnectUpperNode();
    movedNode->close();
    if(from!=NodeWidget::mainWindow->getMap())
        from->insert(fromIndex, movedNode);
    else{
        from->onlyInsert(fromIndex, movedNode);

        QQueue<NodeWidget*> queue;
        NodeWidget* temp;

        queue.push_back(movedNode);
        movedNode->label().setDefaultColor(fromColor);
        QColor col(movedNode->label().getDefaultColorString());

        while(!queue.empty()){
            temp = queue.front();

            temp->label().setDefaultColor(fromColor);
            temp->getPen().setColor(col);

            queue.pop_front();

            for(int i = 0; i<temp->getChild().count();i++)
                queue.push_back(temp->getChild()[i]);
        }
    }
    movedNode->show();
}

void MoveCommand::redo(){
    movedNode->disconnectUpperNode();
    movedNode->close();
    if(to!=NodeWidget::mainWindow->getMap())
        to->insert(toIndex, movedNode);
    else{
        to->onlyInsert(toIndex, movedNode);

        QQueue<NodeWidget*> queue;
        NodeWidget* temp;

        queue.push_back(movedNode);
        movedNode->label().setDefaultColor(toColor);
        QColor col(movedNode->label().getDefaultColorString());

        while(!queue.empty()){
            temp = queue.front();

            temp->label().setDefaultColor(toColor);
            temp->getPen().setColor(col);

            queue.pop_front();

            for(int i = 0; i<temp->getChild().count();i++)
                queue.push_back(temp->getChild()[i]);
        }
    }
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

NodeStyleCommand::NodeStyleCommand(NodeWidget *styleChangedNode, nodeShape shape) : Command(CommandType::NodeStyle){
    this->styleChangedNode = styleChangedNode;
    this->shape = shape;
}

void NodeStyleCommand::undo(){
    QString textColor = styleChangedNode->label().getNodeTextColor();
    switch(shape){
    case rec:
        if(styleChangedNode->label().getNodeShape()==rec)
            styleChangedNode->label().setNodeShape(nothing);
        else
            styleChangedNode->label().setNodeShape(rec);
        break;
    case underline:
        if(styleChangedNode->label().getNodeShape()==underline)
            styleChangedNode->label().setNodeShape(nothing);
        else
            styleChangedNode->label().setNodeShape(underline);
        break;
    case roundRec:
        if(styleChangedNode->label().getNodeShape()==roundRec)
            styleChangedNode->label().setNodeShape(nothing);
        else
            styleChangedNode->label().setNodeShape(roundRec);
        break;
    }
    QString shapeOfNode = styleChangedNode->label().getNodeShapeCSS();
    QString borderColor = styleChangedNode->label().getDefaultColorCSS();
    styleChangedNode->label().setStyleSheet(shapeOfNode+textColor+borderColor+"background-color : #6699ff;");
}

void NodeStyleCommand::redo(){
    undo();
}

ImageCommand::ImageCommand(NodeWidget *imageInsertedNode) : Command(CommandType::Image){
    this->imageInsertedNode = imageInsertedNode;
    image = imageInsertedNode->getImage();
    text = imageInsertedNode->getEdit().getSavedText();
}

void ImageCommand::undo(){
    imageInsertedNode->label().clear();
    imageInsertedNode->label().setText(text);
}

void ImageCommand::redo(){
    imageInsertedNode->imageResize();
}
