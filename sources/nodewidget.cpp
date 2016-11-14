#include "headers/nodewidget.h"
#include "headers/mainwindow.h"

void NodeLabel::mousePressEvent(QMouseEvent *e){
    if(focus){
        emit doubleClicked();
    }
    else{
        this->focusIn();
    }
}

void NodeLabel::keyPressEvent(QKeyEvent *e){
    switch(e->key()){
    case Qt::Key_Tab:
        emit tabPressed();
        break;
    case Qt::Key_Enter:
        emit enterPressed();
        break;
    case Qt::Key_Return:
        emit enterPressed();
        break;
    case Qt::Key_Delete:
        emit deletePressed();
        break;
    case Qt::Key_Left:
        emit arrowPressed(Qt::Key_Left);
        break;
    case Qt::Key_Right:
        emit arrowPressed(Qt::Key_Right);
        break;
    case Qt::Key_Up:
        emit arrowPressed(Qt::Key_Up);
        break;
    case Qt::Key_Down:
        emit arrowPressed(Qt::Key_Down);
        break;
    default:
        emit keyPressed();
        QLabel::keyPressEvent(e);
        break;
    }
}

void NodeLabel::focusOutEvent(QFocusEvent *e){
    this->focusOut();
}

void NodeLabel::focusIn(){
    this->setFocus();
    focus = true;
    this->setStyleSheet("border: 4px solid gray;");
    emit redraw();
}

void NodeLabel::focusOut(){
    focus = false;
    this->setStyleSheet("border: 2px solid gray;");
    emit redraw();
}

void NodeTextEdit::keyPressEvent(QKeyEvent *e){
    if(e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return){
        if(e->modifiers().testFlag(Qt::ShiftModifier)){
            QTextEdit::keyPressEvent(e);
        }
        else{
            emit enterPressed();
        }
    }
    else if(e->key() == Qt::Key_Tab);
    else{
        QTextEdit::keyPressEvent(e);
    }
}

void NodeTextEdit::focusOutEvent(QFocusEvent *e){
    emit focusOut();
}

QString NodeTextEdit::text(){
    QString temp = "";
    for(int i=0;i<textVector_.count();i++)
        temp += textVector_[i];
    return temp;
}

QString NodeTextEdit::labelText(){
    QString temp = "";
    for(int i=0;i<textVector_.count()-1;i++){
        temp += textVector_[i] + "\n";
    }
    temp += textVector_.last();
    return temp;
}

NodeWidget::NodeWidget(QString name){
    init();
    selfWidget.setText(name);
    edit.saveText(name);
}

NodeWidget::NodeWidget(QQueue<MdString> list, MainWindow* mainWindow){
    init();
    this->mainWindow = mainWindow;
    QObject::connect(&edit,SIGNAL(enterPressed()),mainWindow,SLOT(renewTextEdit()));
    if(list.isEmpty()){
        selfWidget.setText("ERROR: not valid .md file");
        return;
    }
    else{
        QString temp;
        selfWidget.setText(temp = list.dequeue().getText());
        edit.saveText(temp);
        NodeWidget* ptr = this;
        int currentDepth = 0;
        while(!list.isEmpty()){
            int temp = list.first().getDepth();
            if(temp == currentDepth+1){
                MdString tempMdString = list.dequeue();
                temp = tempMdString.getDepth();
                NodeWidget* tempnode = new NodeWidget(tempMdString.getText());
                QObject::connect(&tempnode->getEdit(),SIGNAL(enterPressed()),mainWindow,SLOT(renewTextEdit()));
                ptr->add(tempnode);
                ptr = tempnode;
                currentDepth++;
            }
            else{
                ptr = ptr->getParent();
                currentDepth--;
            }
        }
    }
}

NodeWidget* NodeWidget::getRoot(){
    NodeWidget* temp;
    temp = this;
    while(temp->parent_!=nullptr)
        temp = temp->parent_;
    return temp;
}

NodeWidget::~NodeWidget(){
    int num = child.count();
    delete fm;
    for(int i=0;i<num;i++){
        delete child[i];
    }
}

void NodeWidget::add(NodeWidget *subNodeWidget){
    child.push_back(subNodeWidget);
    childLayout.addWidget(subNodeWidget);
    subNodeWidget->parent_ = this;
    subNodeWidget->mainWindow = mainWindow;
    subNodeWidget->index = child.count()-1;
}

void NodeWidget::insert(int index, NodeWidget *subNode){
    child.insert(index, subNode);
    childLayout.insertWidget(index, subNode);
    for(int i=index+1;i<child.count();i++)
        child[i]->index++;
    subNode->parent_ = this;
    subNode->mainWindow = mainWindow;
    subNode->index = index;
}

NodeWidget* NodeWidget::searchFocusInNode(NodeWidget* root){
    QQueue<NodeWidget*> queue;
    NodeWidget* temp;
    int i;

    if(root == nullptr)
        return nullptr;
    queue.push_back(root);

    while(!queue.empty()){
        temp = queue.front();
        queue.pop_front();
        if(temp->editMode){
            return temp;
        }
        else{
            if(temp->selfWidget.isFocus())
                return temp;
        }
        for(i = 0; i<temp->child.count();i++)
            queue.push_back(temp->child[i]);
    }
    return nullptr;
}

void NodeWidget::labelToTextEdit(){
    editMode = true;
    edit.setReadOnly(false);
    edit.setText(edit.getSavedText());
    textEditSizeRenew();
    edit.selectAll();
    selfWidget.hide();
    delete layout.takeAt(0);
    layout.insertWidget(0,&edit);
    edit.show();
    edit.setFocus();
}

void NodeWidget::textEditToLabel(){
    if(editMode){
        editMode = false;
        selfWidget.show();
        selfWidget.setText(edit.labelText());
        delete layout.takeAt(0);
        layout.insertWidget(0,&selfWidget);
        edit.setText("");
        edit.setReadOnly(true);
        edit.hide();
    }
}

void NodeWidget::textEditSizeRenew(){
    int charCount;
    int y = 1;
    QString temp = "";
    QString text;
    QStringList spaceSplit;
    if(editMode){
        edit.textVector().clear();
        text = edit.toPlainText();
        edit.saveText(text);
        charCount = text.count();
        for(int i=0;i<charCount;i++){
            temp += text[i];
            if(text[i] == '\n'){
                y++;
                edit.textVector().append("");
                for(int i=0;i<temp.count()-1;i++)
                    edit.textVector()[edit.textVector().count()-1]+=temp[i];
                temp = "";
                continue;
            }
            if(fm->width(temp) > 100){
                spaceSplit = temp.split(' ');
                if(spaceSplit.count() == 1){
                    edit.textVector().append("");
                    for(int i=0;i<temp.count()-1;i++)
                        edit.textVector()[edit.textVector().count()-1]+=temp[i];
                    temp = temp[temp.count()-1];
                    y++;
                }
                else{
                    edit.textVector().append("");
                    for(int i=0;i<spaceSplit.count()-2;i++)
                        edit.textVector()[edit.textVector().count()-1]+=spaceSplit[i] + " ";
                    edit.textVector()[edit.textVector().count()-1]+=spaceSplit[spaceSplit.count()-2];
                    temp = spaceSplit.last();
                    y++;
                }
            }
        }
        edit.textVector().append(temp);
        if(y == 1){
            edit.setFixedSize(fm->width(temp) + 30, fm->height() + 12);
            edit.setFixedSize(fm->width(temp) + 10, fm->height() + 12);
        }
        else{
            edit.setFixedSize(130, (fm->height()+2)*y + 10);
            edit.setFixedSize(110, (fm->height()+2)*y + 10);
        }
    }
}

void NodeWidget::makeDefaultChildNode(){
    NodeWidget* child = new NodeWidget("default");
    add(child);
    child->label().focusIn();
    selfWidget.focusOut();
}

void NodeWidget::makeDefaultSiblingNode(){
    if(parent_!=nullptr){
        NodeWidget* child = new NodeWidget("default");
        parent_->insert(index + 1, child);
        child->label().focusIn();
        selfWidget.focusOut();
    }
}

void NodeWidget::deleteThisNode(){
    this->~NodeWidget();
    if(parent_ != nullptr){
        parent_->child.removeAt(index);
        for(int i=index;i<parent_->child.count();i++)
            parent_->child[i]->index--;
    }
    getRoot()->update();
}

void NodeWidget::focusMoveByArrow(int key){
    switch(key){
    case Qt::Key_Left:
        if(parent_!=nullptr){
            selfWidget.focusOut();
            parent_->label().focusIn();
        }
        break;
    case Qt::Key_Right:
        if(child.count()!=0){
            selfWidget.focusOut();
            child[0]->label().focusIn();
        }
        break;
    case Qt::Key_Up:
        if(index!=0){
            selfWidget.focusOut();
            parent_->child[index-1]->label().focusIn();
        }
        break;
    case Qt::Key_Down:
        if(parent_!=nullptr && this!=parent_->child.last()){
            selfWidget.focusOut();
            parent_->child[index+1]->label().focusIn();
        }
        break;
    }
}

void NodeWidget::paintEvent(QPaintEvent *e){
    QPainter painter(this);
    QPen myPen;
    myPen.setWidth(2);
    painter.setPen(myPen);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);

    //painter.drawLine();
    //painter.drawLine(selfWidget.mapToParent(selfWidget.rect().bottomLeft()),selfWidget.mapToParent(selfWidget.rect().bottomRight()));
    for(int i = 0 ; i<child.count() ; i++){
        QPoint temp;
        temp = selfWidget.mapToGlobal(selfWidget.rect().bottomRight());
        QPoint pos1 = this->mapFromGlobal(temp);
        temp = child[i]->selfWidget.mapToGlobal(child[i]->selfWidget.rect().bottomLeft());
        QPoint pos2 = this->mapFromGlobal(temp);


        QPainterPath path;
        path.moveTo(pos1);
        path.cubicTo(pos1+QPoint(10,0),pos2+QPoint(-10,0),pos2);
        painter.drawPath(path);
    }
}
