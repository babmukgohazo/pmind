#include "headers/mainwindow.h"

void NodeLabel::mousePressEvent(QMouseEvent *e){
    prePos = e->pos();
    if(focus){
        emit doubleClicked();
    }
    else{
        this->focusIn();
    }
}

void NodeLabel::keyPressEvent(QKeyEvent *e){
    if(!focus)
        return;
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
    case Qt::Key_Z:
    case Qt::Key_Y:
    case Qt::Key_Q:
    case Qt::Key_W:
    case Qt::Key_E:
    case Qt::Key_R:
    case Qt::Key_T:
    case Qt::Key_U:
    case Qt::Key_I:
    case Qt::Key_O:
    case Qt::Key_P:
    case Qt::Key_A:
    case Qt::Key_S:
    case Qt::Key_D:
    case Qt::Key_F:
    case Qt::Key_G:
    case Qt::Key_H:
    case Qt::Key_J:
    case Qt::Key_K:
    case Qt::Key_L:
    case Qt::Key_X:
    case Qt::Key_C:
    case Qt::Key_V:
    case Qt::Key_B:
    case Qt::Key_N:
    case Qt::Key_M:
    case Qt::Key_0:
    case Qt::Key_1:
    case Qt::Key_2:
    case Qt::Key_3:
    case Qt::Key_4:
    case Qt::Key_5:
    case Qt::Key_6:
    case Qt::Key_7:
    case Qt::Key_8:
    case Qt::Key_9:
        emit keyPressed();
        break;
    default:
        QLabel::keyPressEvent(e);
        break;
    }
}

void NodeLabel::focusOutEvent(QFocusEvent *e){
    //this->focusOut();
}

void NodeLabel::focusIn(){
    NodeWidget* temp;
    temp = NodeWidget::searchFocusInNode(container_->getRoot());
    if(temp != nullptr)
        temp->label().focusOut();
    this->setFocus();
    focus = true;
    QString shapeTmp =this->getNodeShapeCSS();//모양을 얻어온다
    QString colorTmp =this->getNodeTextColor();//글자 색을 얻어온다
    this->setStyleSheet(shapeTmp+colorTmp+"background-color : #6699ff;"); //바탕화면 파란색
    emit focused();
    emit redraw();
}

void NodeLabel::focusOut(){
    focus = false;
    QString shapeTmp =this->getNodeShapeCSS();
    QString colorTmp =this->getNodeTextColor();
    this->setStyleSheet(shapeTmp+colorTmp+"background-color : #ffffff;"); //바탕화면 하얀색으로 돌리기
    emit noFocused();
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
    else if(e->key() == Qt::Key_Escape){
        emit escPressed();
    }
    else{
        QTextEdit::keyPressEvent(e);
    }
}

void NodeTextEdit::focusOutEvent(QFocusEvent *e){
    emit focusOut();
}

void NodeLabel::mouseMoveEvent(QMouseEvent *event){
    if(QLineF(prePos, event->pos()).length() > 10){
        QDrag *drag = new QDrag(this);
        QMimeData *mime = new QMimeData;
        drag->setMimeData(mime);
        mime->setColorData(qVariantFromValue((void*)parent()));

        drag->setPixmap(((QWidget*)parent())->grab());
        drag->setHotSpot((static_cast<QWidget*>(parent()))->mapFromGlobal(mapToGlobal(event->pos())));
        drag->exec();
        focusOut();
        setCursor(Qt::OpenHandCursor);

    }
}

void NodeLabel::dragEnterEvent(QDragEnterEvent *event){
    event->accept();
    event->setAccepted(true);
    dragOver = true;
    update();
}

void NodeLabel::dragMoveEvent(QDragMoveEvent *event){
    event->accept();
}

void NodeLabel::dragLeaveEvent(QDragLeaveEvent *event){
    Q_UNUSED(event);
    dragOver = false;
    update();
}

void NodeLabel::dropEvent(QDropEvent *event){
    //resize(size() * 1.1);
    dragOver = false;
    if (event->mimeData()->hasColor()){
        NodeWidget* temp1 = ((NodeWidget*)parent());
        void* temp = qvariant_cast<void*>(event->mimeData()->colorData());
        NodeWidget* temp2 = (NodeWidget*)temp;
        if(temp1==temp2)
            return;
        if(!(temp1->isChildOf(temp2))){
            emit commanded(temp2,temp1,CommandType::Move);
            temp2 = temp2->takeNode();
            temp1->insert(temp1->getChild().size(),temp2);
        }
    }
    update();
}

QString NodeLabel::getNodeShapeCSS(){
    switch(nodeShape){
    case rec:
        return recCSS;
    case roundRec:
        return roundRecCSS;
    case underline:
        return underlineCSS;
    default:
        return nothingCSS;
    }
}

QString NodeTextEdit::labelText(){
    QString temp = "";
    for(int i=0;i<textVector_.count()-1;i++){
        temp += textVector_[i] + "\n";
    }
    temp += textVector_.last();
    return temp;
}

MainWindow* NodeWidget::mainWindow = nullptr;

NodeWidget::NodeWidget(QString name){
    init();
    selfWidget.setText(name);
    edit.saveText(name);
}

NodeWidget::NodeWidget(QQueue<MdString> list, MainWindow* mainWindow){
    NodeWidget::mainWindow = mainWindow;
    init();
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

int NodeWidget::getDepth(){
    NodeWidget* temp;
    int depth = 0;
    temp = this;
    while(temp->parent_!=nullptr) {
        temp = temp->parent_;
        depth = depth + 1;
    }
    return depth;
}

void NodeWidget::init(){
//<<<<<<< HEAD
    edit.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    edit.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//=======
    dockWidget=mainWindow->getDockWidget();
//>>>>>>> feature/sprint_3_dockWidget
    fm = new QFontMetrics(edit.currentFont());
    selfWidget.setContainer(this);
    this->setStyleSheet("background-color: transparent");
    //selfWidget.setStyleSheet("background-color: transparent ; border-bottom: 1px solid black;");
    selfWidget.setStyleSheet("border: 2px solid gray;");
//<<<<<<< HEAD
    selfWidget.setSizePolicy(QSizePolicy::QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    childWidget.setSizePolicy(QSizePolicy::QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed));
//=======
    //selfWidget.setSizePolicy(QSizePolicy::QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
//>>>>>>> feature/sprint_3_dockWidget
    layout.addWidget(&selfWidget);
    layout.addWidget(&childWidget);
    layout.setSpacing(30);
    this->setLayout(&layout);
    childWidget.setLayout(&childLayout);
    layout.setContentsMargins(0,0,0,0);
    childLayout.setMargin(0);
    QObject::connect(&edit,SIGNAL(enterPressed()),this,SLOT(textEditToLabel()));
    QObject::connect(&edit,SIGNAL(enterPressed()),&selfWidget,SLOT(focusIn()));
    QObject::connect(&edit,SIGNAL(focusOut()),this,SLOT(textEditToLabel()));
    QObject::connect(&edit,SIGNAL(focusOut()),NodeWidget::mainWindow,SLOT(renewTextEdit()));
    QObject::connect(&selfWidget,SIGNAL(doubleClicked()),this,SLOT(labelToTextEdit()));
    QObject::connect(&edit,SIGNAL(textChanged()),this,SLOT(textEditSizeRenew()));
    QObject::connect(&selfWidget,SIGNAL(tabPressed()),this,SLOT(makeDefaultChildNode()));
    QObject::connect(&selfWidget,SIGNAL(enterPressed()),this,SLOT(makeDefaultSiblingNode()));
    QObject::connect(&selfWidget,SIGNAL(deletePressed()),this,SLOT(deleteFromMap()));
    QObject::connect(&selfWidget,SIGNAL(keyPressed()),this,SLOT(labelToTextEdit()));
    QObject::connect(&selfWidget,SIGNAL(arrowPressed(int)),this,SLOT(focusMoveByArrow(int)));
    QObject::connect(&selfWidget,SIGNAL(redraw()),getRoot(),SLOT(update()));
    QObject::connect(&edit,SIGNAL(escPressed()),this,SLOT(closeTextEdit()));
    QObject::connect(&edit,SIGNAL(escPressed()),&selfWidget,SLOT(focusIn()));
    QObject::connect(this,SIGNAL(commanded(NodeWidget*,CommandType)),NodeWidget::mainWindow,SLOT(addProcess(NodeWidget*,CommandType)));
    QObject::connect(&selfWidget,SIGNAL(commanded(NodeWidget*,NodeWidget*,CommandType)),this,SIGNAL(commanded(NodeWidget*,NodeWidget*,CommandType)));
    QObject::connect(this,SIGNAL(commanded(NodeWidget*,NodeWidget*,CommandType)),NodeWidget::mainWindow,SLOT(addProcess(NodeWidget*,NodeWidget*,CommandType)));
    QObject::connect(&selfWidget,SIGNAL(focused()),dockWidget,SLOT(showAllProperty()));
    QObject::connect(&selfWidget,SIGNAL(focused()),dockWidget,SLOT(propertyEnabled()));
    QObject::connect(&selfWidget,SIGNAL(noFocused()),dockWidget,SLOT(propertyUnEnabled()));
    QObject::connect(this,SIGNAL(generated()),NodeWidget::mainWindow,SLOT(renewTextEdit()));
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
    subNodeWidget->index = child.count()-1;
    emit generated();
}

void NodeWidget::insert(int index, NodeWidget *subNode){
    child.insert(index, subNode);
    childLayout.insertWidget(index, subNode);
    for(int i=index+1;i<child.count();i++)
        child[i]->index++;
    subNode->parent_ = this;
    subNode->index = index;
    emit generated();
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
    selfWidget.close();
    delete layout.takeAt(0);
    layout.insertWidget(0,&edit);
    edit.show();
    edit.setFocus();
}

void NodeWidget::textEditToLabel(){
    if(editMode){
        if(edit.getSavedText()!=edit.toPlainText()){
            emit commanded(this, CommandType::Text);
            edit.saveText(edit.toPlainText());
        }

        editMode = false;
        selfWidget.show();
        labelSizeRenew();
        delete layout.takeAt(0);
        layout.insertWidget(0,&selfWidget);
        edit.close();
    }
}

void NodeWidget::closeTextEdit(){
    if(editMode){
        editMode = false;
        selfWidget.show();
        delete layout.takeAt(0);
        layout.insertWidget(0,&selfWidget);
        edit.close();
    }
}

void NodeWidget::textEditSizeRenew(){
    if(editMode){
        QString text = edit.toPlainText();
        int x = fm->width(text);
        int n = x/300 + 1;
        if(x<=300){
            edit.setFixedWidth(x + 10);
            int y = edit.document()->size().height();
            if(x<=30)
                x=30;
            if(y==0)
                y=fm->height() + 12;
            edit.setFixedSize(x + 10, y);
        }
        else{
            int y = edit.document()->size().height();
            edit.setFixedSize(310,y);
        }
    }
    this->update();
}

void NodeWidget::labelSizeRenew(){
    editMode = true;
    int charCount;
    QString temp = "";
    QString text;
    QStringList spaceSplit;
    if(editMode){
        edit.textVector().clear();
        text = edit.getSavedText();
        charCount = text.count();
        for(int i=0;i<charCount;i++){
            temp += text[i];
            if(text[i] == '\n'){
                edit.textVector().append("");
                for(int i=0;i<temp.count()-1;i++)
                    edit.textVector()[edit.textVector().count()-1]+=temp[i];
                temp = "";
                continue;
            }
            if(fm->width(temp) > 300){
                spaceSplit = temp.split(' ');
                if(spaceSplit.count() == 1){
                    edit.textVector().append("");
                    for(int i=0;i<temp.count()-1;i++)
                        edit.textVector()[edit.textVector().count()-1]+=temp[i];
                    temp = temp[temp.count()-1];
                }
                else{
                    edit.textVector().append("");
                    for(int i=0;i<spaceSplit.count()-2;i++)
                        edit.textVector()[edit.textVector().count()-1]+=spaceSplit[i] + " ";
                    edit.textVector()[edit.textVector().count()-1]+=spaceSplit[spaceSplit.count()-2];
                    temp = spaceSplit.last();
                }
            }
        }
        edit.textVector().append(temp);
    }
    selfWidget.setText(edit.labelText());
    editMode = false;
}

void NodeWidget::makeDefaultChildNode(){
    NodeWidget* child = new NodeWidget("default");
    add(child);
    emit commanded(child, CommandType::Add);
    child->label().focusIn();
    selfWidget.focusOut();
}

void NodeWidget::makeDefaultSiblingNode(){
    if(parent_!=nullptr){
        NodeWidget* child = new NodeWidget("default");
        parent_->insert(index + 1, child);
        emit commanded(child, CommandType::Add);
        child->label().focusIn();
        selfWidget.focusOut();
    }
}

void NodeWidget::deleteFromMap(){
    if(parent_==nullptr)
        return;
    emit commanded(this, CommandType::Delete);
    disconnectUpperNode();
    this->close();
    selfWidget.focusOut();
}

void NodeWidget::disconnectUpperNode(){
    if(parent_ != nullptr){
        parent_->child.removeAt(index);
        for(int i=index;i<parent_->child.count();i++)
            parent_->child[i]->index--;
        parent_=nullptr;
    }
    emit generated();
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

    QPoint pos1, pos2;

    //painter.drawLine();
    //painter.drawLine(selfWidget.mapToParent(selfWidget.rect().bottomLeft()),selfWidget.mapToParent(selfWidget.rect().bottomRight()));

    if(editMode){
        for(int i = 0; i<child.count(); i++){
            QPoint temp;
            temp = edit.mapToGlobal(edit.rect().bottomRight());
            pos1 = this->mapFromGlobal(temp);
            temp = child[i]->selfWidget.mapToGlobal(child[i]->selfWidget.rect().bottomLeft());
            pos2 = this->mapFromGlobal(temp);


            QPainterPath path;
            path.moveTo(pos1);
            path.cubicTo(pos1+QPoint(10,0),pos2+QPoint(-10,0),pos2);
            painter.drawPath(path);
        }
    }
    else{
        for(int i = 0 ; i<child.count() ; i++){
            QPoint temp;
            if(child[i]->editMode){
                temp = selfWidget.mapToGlobal(selfWidget.rect().bottomRight());
                pos1 = this->mapFromGlobal(temp);
                temp = child[i]->edit.mapToGlobal(child[i]->edit.rect().bottomLeft());
                pos2 = this->mapFromGlobal(temp);
            }
            else{
                temp = selfWidget.mapToGlobal(selfWidget.rect().bottomRight());
                pos1 = this->mapFromGlobal(temp);
                temp = child[i]->selfWidget.mapToGlobal(child[i]->selfWidget.rect().bottomLeft());
                pos2 = this->mapFromGlobal(temp);
            }

            QPainterPath path;
            path.moveTo(pos1);
            path.cubicTo(pos1+QPoint(10,0),pos2+QPoint(-10,0),pos2);
            painter.drawPath(path);
        }
    }
}

NodeWidget* NodeWidget::takeNode(){
    if(parent_ != nullptr){
        parent_->child.removeAt(index);
        for(int i=index;i<parent_->child.count();i++)
            parent_->child[i]->index--;
    }
    getRoot()->update();
    return this;
}

bool NodeWidget::isChildOf(NodeWidget* ptr){
    if(parent_== nullptr)
        return false;
    else if(parent_ == ptr)
        return true;
    else
        return parent_->isChildOf(ptr);
}

//<<<<<<< HEAD
void NodeWidget::setEditFont(const QFont &font){
    delete fm;
    fm = new QFontMetrics(font);
    this->font = font;
    edit.setFont(font);
    labelSizeRenew();
    editMode = true;
    textEditSizeRenew();
    editMode = false;
}


//=======
//>>>>>>> feature/sprint_3_dockWidget
