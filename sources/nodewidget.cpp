#include "headers/mainwindow.h"
#include "headers/nodewidget.h"

int NodeWidget::counter = 0;

void NodeLabel::mousePressEvent(QMouseEvent *e){
    prePos = e->pos();
    if(e->button()==Qt::LeftButton)
        emit labelClicked();
}

void NodeLabel::mouseReleaseEvent(QMouseEvent *e){
    if(e->button()==Qt::LeftButton){
        if(focus){
            emit doubleClicked();
        }
        else{
            focusIn();
        }
    }
    else if(e->button()==Qt::RightButton){
        if(!focus)
            focusIn();
    }
}

void NodeLabel::mouseDoubleClickEvent(QMouseEvent *e){
    if(e->button()==Qt::LeftButton){
        focusIn();
        emit doubleClicked();
    }
}

void NodeLabel::keyPressEvent(QKeyEvent *e){
    if(e->modifiers().testFlag(Qt::ControlModifier)|e->modifiers().testFlag(Qt::ShiftModifier)){
        if(e->key()==Qt::Key_I){
            if(e->modifiers().testFlag(Qt::ControlModifier))
                emit italic();
        }
        else if(e->key()==Qt::Key_B){
            if(e->modifiers().testFlag(Qt::ControlModifier))
                emit bold();
        }
        QLabel::keyPressEvent(e);
        return;
    }

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
    case Qt::Key_I:
    case Qt::Key_B:
    case Qt::Key_Z:
    case Qt::Key_Y:
    case Qt::Key_Q:
    case Qt::Key_W:
    case Qt::Key_E:
    case Qt::Key_R:
    case Qt::Key_T:
    case Qt::Key_U:
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
   // focusOut();
}

void NodeLabel::focusIn(){
    NodeWidget* temp;
    temp = NodeWidget::searchFocusInNode(container_->getRoot());
    if(temp != nullptr)
        temp->label().focusOut();
    this->setFocus();
    focus = true;
    if(static_cast<NodeWidget*>(parent())->isImageMode())
        setNodeShape(rec);
    QString shapeTmp =this->getNodeShapeCSS();//모양을 얻어온다
    QString colorTmp =this->getNodeTextColor();//글자 색을 얻어온다
    QString borderTmp = this->getDefaultColorCSS();//노드의 default 색깔 값을 얻어온다.
    this->setStyleSheet(shapeTmp+colorTmp+borderTmp+"background-color : #6699ff;"); //바탕화면 파란색
    emit focused();
    emit redraw();
    if(static_cast<NodeWidget*>(parent())->isImageMode())
        emit inImageMode();
}

void NodeLabel::focusOut(){
//<<<<<<< HEAD
    if(focus){
        focus = false;
        if(static_cast<NodeWidget*>(parent())->isImageMode())
            setNodeShape(nothing);
        QString shapeTmp =this->getNodeShapeCSS();
        QString colorTmp =this->getNodeTextColor();
        QString borderTmp = this->getDefaultColorCSS();//노드의 default 색깔 값을 얻어온다.
        this->setStyleSheet(shapeTmp+colorTmp+borderTmp+"background-color : transparent;"); //바탕화면 하얀색으로 돌리기
        emit noFocused();
        emit redraw();
    }
//=======
//    focus = false;
//    QString shapeTmp =this->getNodeShapeCSS();
//    QString colorTmp =this->getNodeTextColor();
//    QString borderTmp = this->getDefaultColorCSS();//노드의 default 색깔 값을 얻어온다.
//    this->setStyleSheet(shapeTmp+colorTmp+borderTmp+"background-color : #ffffff;"); //바탕화면 하얀색으로 돌리기
//    emit noFocused();
//    emit redraw();
//>>>>>>> feature/sprint3_nodeColor_Gyuyong
}

QString NodeLabel::getDefaultColorCSS(){
    switch(defaultColor){
    case blue:
        defaultColorCSS="border-color: #298aab;";
        break;
    case red:
        defaultColorCSS="border-color: #e55251;";
        break;
    case green:
        defaultColorCSS="border-color: #41a441;";
        break;
    case orange:
        defaultColorCSS="border-color: #e79527;";
        break;
    case yellow:
        defaultColorCSS="border-color: #fee13e;";
        break;
    case mint:
        defaultColorCSS="border-color: #37aea1;";
        break;
    case 100:
        defaultColorCSS="border-color: #aed339;";
    }
    return defaultColorCSS;
}

QString NodeLabel::getDefaultColorString()
{
    switch(defaultColor){
    case blue:
        defaultColorString="#298aab";
        break;
    case red:
        defaultColorString="#e55251";
        break;
    case green:
        defaultColorString="#41a441";
        break;
    case orange:
        defaultColorString="#e79527";
        break;
    case yellow:
        defaultColorString="#fee13e";
        break;
    case mint:
        defaultColorString="#37aea1";
        break;
    case 100:
        defaultColorString="#aed339";
    }
    return defaultColorString;
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

void NodeTextEdit::mousePressEvent(QMouseEvent *e){
    emit editClicked();
    QTextEdit::mousePressEvent(e);
}

void NodeLabel::mouseMoveEvent(QMouseEvent *event){
    if(static_cast<NodeWidget*>(parent())==NodeWidget::mainWindow->getMap())
        return;

    if(QLineF(prePos, event->pos()).length() > 5){
        QDrag *drag = new QDrag(this);
        QMimeData *mime = new QMimeData;
        drag->setMimeData(mime);
        mime->setColorData(qVariantFromValue((void*)parent()));

        MindmapView* mapScreen = NodeWidget::mainWindow->getMapScreen();
        QWidget* con = NodeWidget::mainWindow->getContainer();
        NodeWidget* parent_ = static_cast<NodeWidget*>(parent());
        int cx,cy;
        int x,y;
        cx = con->rect().width();
        cy = con->rect().height();
        x = parent_->rect().width();
        y = parent_->rect().height();

        qreal scale;
        scale = mapScreen->getCurrentScale()/100;
        cx *= scale;
        cy *= scale;
        x *= scale;
        y *= scale;

        QImage image(QSize(cx,cy),QImage::Format_ARGB32);
        image.fill(Qt::transparent);
        QPainter painter(&image);
        painter.setRenderHint(QPainter::Antialiasing);
        QRect rect;
        QPoint origin = parent_->mapToGlobal(QPoint(0,0));
        origin = con->mapFromGlobal(origin);
        origin *= scale;
        rect.setRect(origin.x(),origin.y(),x,y);
        mapScreen->getScene()->render(&painter);
        QImage cutImage = image.copy(rect);

        drag->setPixmap(QPixmap::fromImage(cutImage));
        if(parent_->isImageMode())
            drag->setHotSpot((static_cast<QWidget*>(parent()))->mapFromGlobal(mapToGlobal(QPoint(0,0))));
        else
            drag->setHotSpot((static_cast<QWidget*>(parent()))->mapFromGlobal(mapToGlobal(event->pos()))*scale);
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
        if(temp1==temp2->getParent())
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
    case root:
        return rootCSS;
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

int NodeWidget::getDefaultColor(){
    return selfWidget.getDefaultColor();
}

void NodeWidget::init(){
    edit.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    edit.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    PropertyDock=mainWindow->getPropertyDock();
    textViewDock=mainWindow->getTextViewDock();
    edit.setContextMenuPolicy(Qt::NoContextMenu);
    selfWidget.setContainer(this);
    this->setStyleSheet("background-color: transparent");
    //selfWidget.setStyleSheet("background-color: transparent ; border-bottom: 1px solid black;");
    //selfWidget.setStyleSheet("border: 2px solid gray;");
    selfWidget.setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    childWidget.setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    layout.addWidget(&selfWidget);
    layout.addWidget(&childWidget);
    layout.setSpacing(30);
    this->setLayout(&layout);
    childWidget.setLayout(&childLayout);
    layout.setContentsMargins(0,0,0,0);
    childLayout.setMargin(0);

//<<<<<<< HEAD
    selfWidget.setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(&selfWidget, SIGNAL(customContextMenuRequested(const QPoint &)),this,SLOT(showCustomMenu(const QPoint&)));
//=======
    QFont *font = new QFont("Arial");
    font->setPointSize(14);
    this->font = *font;
    fm = new QFontMetrics(*font);
    selfWidget.setFont(*font);
    edit.setFont(*font);
    textEditSizeRenew();
//>>>>>>> feature/sprint3_fontFix

    pen.setWidth(2);

    QObject::connect(&edit,SIGNAL(enterPressed()),NodeWidget::mainWindow,SLOT(renewTextEdit()));
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
    QObject::connect(&selfWidget,SIGNAL(focused()),PropertyDock,SLOT(showAllProperty()));
    QObject::connect(&selfWidget,SIGNAL(focused()),PropertyDock,SLOT(propertyEnabled()));
    QObject::connect(&selfWidget,SIGNAL(noFocused()),PropertyDock,SLOT(propertyUnEnabled()));
    QObject::connect(this,SIGNAL(generated()),NodeWidget::mainWindow,SLOT(renewTextEdit()));
    QObject::connect(&edit,SIGNAL(editClicked()),NodeWidget::mainWindow->getMapScreen(),SLOT(editClick()));
    QObject::connect(&selfWidget,SIGNAL(labelClicked()),NodeWidget::mainWindow->getMapScreen(),SLOT(labelClick()));
    QObject::connect(&selfWidget,SIGNAL(italic()),PropertyDock,SLOT(on_buttonItalic_clicked()));
    QObject::connect(&selfWidget,SIGNAL(bold()),PropertyDock,SLOT(on_buttonBold_clicked()));
    QObject::connect(&selfWidget,SIGNAL(inImageMode()),PropertyDock,SLOT(propertyUnEnabled()));
    QObject::connect(&selfWidget,SIGNAL(doubleClicked()),PropertyDock,SLOT(propertyUnEnabled()));
    QObject::connect(&selfWidget,SIGNAL(keyPressed()),PropertyDock,SLOT(propertyUnEnabled()));
}

NodeWidget::~NodeWidget(){
    int num = child.count();
    delete fm;
    for(int i=0;i<num;i++){
        delete child[i];
    }
}
/*
void NodeWidget::setChildDefaultColor(QVector<NodeWidget*> child, int color){
    qDebug() << this;

    for(int i=0;i<child.count();i++)
    {
        if(child[i]->child.empty())
            child[i]->selfWidget.setDefaultColor(color);
        else
        {
            setChildDefaultColor(child[i]->child,color);
            child[i]->selfWidget.setDefaultColor(color);
        }
    }
}
*/
void NodeWidget::add(NodeWidget *subNodeWidget){
    child.push_back(subNodeWidget);
    childLayout.addWidget(subNodeWidget);
    subNodeWidget->parent_ = this;
    subNodeWidget->index = child.count()-1;
    if(subNodeWidget->parent_==getRoot())
    {
        subNodeWidget->selfWidget.setDefaultColor(counter%6);
        counter++;
    }
    else if(subNodeWidget->parent_!=nullptr)//맵이면 안됨
    {
        subNodeWidget->selfWidget.setDefaultColor(this->getDefaultColor());
    }

    QColor* col = new QColor(subNodeWidget->selfWidget.getDefaultColorString());
    subNodeWidget->pen.setColor(*col);
    //qDebug() << selfWidget.font();
    //qDebug() << selfWidget.fontInfo();
    //qDebug() << selfWidget.fontMetrics();
    QFont *childFont = new QFont(subNodeWidget->selfWidget.font());
    QFont *parentFont = new QFont(selfWidget.font());
    childFont->setFamily(parentFont->family());
    subNodeWidget->selfWidget.setFont(*childFont);
    // setFamily 사용해서 글꼴만 수정, fontInfo에서 글꼴 정보 가져오기
    emit generated();
}

void NodeWidget::insert(int index, NodeWidget *subNode){
    child.insert(index, subNode);
    childLayout.insertWidget(index, subNode);
    for(int i=index+1;i<child.count();i++)
        child[i]->index++;
    subNode->parent_ = this;
    subNode->index = index;

//    QQueue<NodeWidget*> queue;
//    NodeWidget* temp;
//    queue.push_back(subNode);


    if(this==getRoot()) // NodeWidget::mainWindow->getMap()
    {
        subNode->selfWidget.setDefaultColor(counter%6);
        counter++;

        QColor* col = new QColor(subNode->selfWidget.getDefaultColorString());
//<<<<<<< HEAD
//=======
        subNode->pen.setColor(*col);

        QFont *childFont = new QFont(subNode->selfWidget.font());
        QFont *parentFont = new QFont(selfWidget.font());
        childFont->setFamily(parentFont->family());
        subNode->selfWidget.setFont(*childFont);
    }

    else if(subNode->parent_!=nullptr)//맵이면 안됨
    {
        QQueue<NodeWidget*> queue;
        NodeWidget* temp;

        queue.push_back(this);

        while(!queue.empty()){
            temp = queue.front();

            temp->selfWidget.setDefaultColor(this->getDefaultColor());

            QColor* col = new QColor(temp->selfWidget.getDefaultColorString());
            temp->pen.setColor(*col);

            QFont *childFont = new QFont(temp->selfWidget.font());
            QFont *parentFont = new QFont(selfWidget.font());
            childFont->setFamily(parentFont->family());
            temp->selfWidget.setFont(*childFont);

            queue.pop_front();

            for(int i = 0; i<temp->child.count();i++)
                queue.push_back(temp->child[i]);
        }

    }
    //QFont *font = new QFont("배달의민족 주아");
    //subNode->selfWidget.setFont(*font);
    emit generated();
}

void NodeWidget::onlyInsert(int index, NodeWidget *subNode){
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
    if(!imageMode){
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
        int space = 11;
        int max;
        bool widthLimit;

        if(font.italic())
            space = 14;

        QStringList enterSplit = text.split('\n');
        QVector<QString> splitVector = enterSplit.toVector();

        if(splitVector.count()<=1){
            if(x>300)
                widthLimit = true;
            else
                widthLimit = false;
            max = x;
        }
        else{
            int temp;

            max = fm->width(splitVector[0]);
            for(int i=1; i<splitVector.count();i++){
                temp = fm->width(splitVector[i]);
                if(max < temp)
                    max = temp;
            }

            if(max>300)
                widthLimit = true;
            else
                widthLimit = false;
        }

        if(widthLimit){
            int y = edit.document()->size().height();
            edit.setFixedSize(300 + space,y);
        }
        else{
            edit.setFixedWidth(max + space);
            int y = edit.document()->size().height();
            if(max<=30)
                max=30;
            if(y==0)
                y=fm->height() + 12;
            edit.setFixedSize(max + space, y);
        }
    }
    NodeWidget::mainWindow->update();
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
    NodeWidget::mainWindow->update();
}

NodeWidget* NodeWidget::getNearestChild(){
    if(child.count()==0)
        return nullptr;
    NodeWidget* nearestChild = child.front();
    QPoint origin = label().mapToGlobal(QPoint(0,0));
    QPoint diff;
    int minLength;
    int length;
    diff = nearestChild->label().mapToGlobal(QPoint(0,0)) - origin;
    minLength = diff.manhattanLength();

    for(int i=1;i<child.count();i++){
        diff = child[i]->label().mapToGlobal(QPoint(0,0)) - origin;
        length = diff.manhattanLength();
        if(length < minLength){
            minLength = length;
            nearestChild = child[i];
        }
    }
    return nearestChild;
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
            getNearestChild()->label().focusIn();
        }
        break;
    case Qt::Key_Up:
        if(parent_==nullptr)
            break;
        selfWidget.focusOut();
        if(index!=0){
            parent_->child[index-1]->label().focusIn();
        }
        else if(index == 0){
            parent_->child.last()->label().focusIn();
        }
        break;
    case Qt::Key_Down:
        if(parent_==nullptr)
                break;
        selfWidget.focusOut();
        if(this!=parent_->child.last()){
            parent_->child[index+1]->label().focusIn();
        }
        else if(this==parent_->child.last()){
            parent_->child.front()->label().focusIn();
        }
        break;
    }
}

void NodeWidget::showCustomMenu(const QPoint &pos){
    QMenu contextMenu(tr("Context menu"));

    QAction action1("Insert image");
    QObject::connect(&action1, SIGNAL(triggered()), this, SLOT(insertImage()));

    contextMenu.addAction(&action1);

    contextMenu.exec(selfWidget.mapToGlobal(pos));
}

void NodeWidget::insertImage(){
    QFileDialog dialog(NodeWidget::mainWindow,
                       tr("Image export"),
                       QDir::homePath(),
                       tr("All (*.*);;PNG (*.png);;JPEG (*.jpeg);;BMP (*.bmp)"));
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setDefaultSuffix("png");
    if (!dialog.exec())
        return;

    QString fileName = dialog.selectedFiles().first();
    image.load(fileName);
    imageResize();

    imageMode = true;
    emit commanded(this,CommandType::Image);

}

void NodeWidget::imageResize(){
    QImage scaledImage;
    int x;
    int y;
    x = image.size().width();
    y = image.size().height();
    double xFrac = x/400;
    double yFrac = y/300;
    if(xFrac>1 | yFrac>1){
        if(xFrac>yFrac){
            x /= xFrac;
            y /= xFrac;
        }
        else{
            x /= yFrac;
            y /= yFrac;
        }
    }
    scaledImage=image.scaled(QSize(x,y),Qt::KeepAspectRatio,Qt::SmoothTransformation);
    selfWidget.setPixmap(QPixmap::fromImage(scaledImage));
}

void NodeWidget::paintEvent(QPaintEvent *e){
    QPainter painter(this);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);

    QPoint pos1, pos2;

    //painter.drawLine();
    //painter.drawLine(selfWidget.mapToParent(selfWidget.rect().bottomLeft()),selfWidget.mapToParent(selfWidget.rect().bottomRight()));
    if(editMode){
        for(int i = 0; i<child.count(); i++){
            painter.setPen(child[i]->getPen());

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
            painter.setPen(child[i]->getPen());

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

void NodeWidget::setEditFont(const QFont &font){
    delete fm;
    this->font = font;
    fm = new QFontMetrics(font);
    this->font = font;
    edit.setFont(font);
    labelSizeRenew();
    editMode = true;
    textEditSizeRenew();
    editMode = false;
}

QPen& NodeWidget::getPen(){
    return pen;
}

void NodeWidget::setPen(QPen& pen){
    this->pen = pen;
    parentWidget()->update();
}
