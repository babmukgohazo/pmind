#include "headers/mainwindow.h"
#include "headers/nodewidget.h"

int NodeWidget::counter = 0;

void NodeLabel::mousePressEvent(QMouseEvent *e){
    prePos = e->pos();
    if(e->button()==Qt::LeftButton)
        emit labelClicked();//mindmapView에게 node가 click되었음을 알림. view에서 focus를 관리하기 때문.
}

void NodeLabel::mouseReleaseEvent(QMouseEvent *e){
    if(e->button()==Qt::LeftButton){
        if(focus){//focus된 상태였으면 textEdit로 전환.
            emit doubleClicked();
        }
        else{//focus아닌 상태였으면 focus.
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
        if(e->key()==Qt::Key_I){//italic font shortcut
            if(e->modifiers().testFlag(Qt::ControlModifier))
                emit italic();
        }
        else if(e->key()==Qt::Key_B){//bold font shortcut
            if(e->modifiers().testFlag(Qt::ControlModifier))
                emit bold();
        }
        QLabel::keyPressEvent(e);
        return;
    }

    if(!focus)
        return;
    switch(e->key()){
    case Qt::Key_Tab://make child
        emit tabPressed();
        break;
    case Qt::Key_Enter://make sibling
        emit enterPressed();
        break;
    case Qt::Key_Return://make sibling
        emit enterPressed();
        break;
    case Qt::Key_Delete://remove node from map
        emit deletePressed();
        break;
    case Qt::Key_Left://make focus move to parent node
        emit arrowPressed(Qt::Key_Left);
        break;
    case Qt::Key_Right://make focus move to nearest child node
        emit arrowPressed(Qt::Key_Right);
        break;
    case Qt::Key_Up://make focus move to upper sibling node
        emit arrowPressed(Qt::Key_Up);
        break;
    case Qt::Key_Down://make focus move to lower sibling node
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
    case Qt::Key_9://if character or number keys press, label is replaced with textEdit
        emit keyPressed();
        break;
    default:
        QLabel::keyPressEvent(e);
        break;
    }
}

void NodeLabel::focusOutEvent(QFocusEvent *e){
   // inactivation
}

void NodeLabel::focusIn(){
    NodeWidget* temp;
    temp = NodeWidget::searchFocusInNode(container_->getRoot());
    if(temp != nullptr)
        temp->label().focusOut();//focus되었던 node에서 focus를 가져온다.
    this->setFocus();
    focus = true;
    if(static_cast<NodeWidget*>(parent())->isImageMode())//image를 가지고 있으면 focus되었을 때 rec형의 테두리가 생긴다.
        setNodeShape(rec);
    QString shapeTmp =this->getNodeShapeCSS();//모양을 얻어온다
    QString colorTmp =this->getNodeTextColor();//글자 색을 얻어온다
    QString borderTmp = this->getDefaultColorCSS();//노드의 default 색깔 값을 얻어온다.
    this->setStyleSheet(shapeTmp+colorTmp+borderTmp+"background-color : #6699ff;"); //바탕화면 파란색
    emit focused();//focus되었다고 propertyDock에 알린다.
    emit redraw();//화면 update
    if(static_cast<NodeWidget*>(parent())->isImageMode())
        emit inImageMode();//image mode에서는 dock이 비활성화 되어야 하므로, image mode인지 propertyDock에 알린다.
}

void NodeLabel::focusOut(){
    if(focus){
        focus = false;
        if(static_cast<NodeWidget*>(parent())->isImageMode())
            setNodeShape(nothing);//node의 모양을 원래 형태로 바꾼다.
        QString shapeTmp =this->getNodeShapeCSS();
        QString colorTmp =this->getNodeTextColor();
        QString borderTmp = this->getDefaultColorCSS();//노드의 default 색깔 값을 얻어온다.
        this->setStyleSheet(shapeTmp+colorTmp+borderTmp+"background-color : transparent;"); //바탕화면 하얀색으로 돌리기
        emit noFocused();//propertyDock inactivation
        emit redraw();//update
    }
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
        if(e->modifiers().testFlag(Qt::ShiftModifier)){//shift+enter해야 줄이 바뀐다.
            QTextEdit::keyPressEvent(e);
        }
        else{
            emit enterPressed();//그냥 enter가 입력되면 textEdit을 종료하고 label로 바뀐다.
        }
    }
    else if(e->key() == Qt::Key_Tab);//tab남용시 windows system에서 자동으로 focus하는 event가 발생한다. 이를 회피하기 위해 비활성화.
    else if(e->key() == Qt::Key_Escape){//textEdit호출된 시점의 text로 복귀하면서 label로 전환.
        emit escPressed();
    }
    else{
        QTextEdit::keyPressEvent(e);
    }
}

void NodeTextEdit::focusOutEvent(QFocusEvent *e){//textEdit에서 label로 전환, dock의 text update
    emit focusOut();
}

void NodeTextEdit::mousePressEvent(QMouseEvent *e){//focus를 관리하는 mindmapView로 focus가 아직 edit에 있는지 확인시켜준다.
    emit editClicked();
    QTextEdit::mousePressEvent(e);
}

void NodeLabel::mouseMoveEvent(QMouseEvent *event){//drag
    if(static_cast<NodeWidget*>(parent())==NodeWidget::mainWindow->getMap())//root node인 경우 drag 불가.
        return;

    if(QLineF(prePos, event->pos()).length() > 5){//특정거리 이상 움직여야 drag가 시작된다.
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

        QImage image(QSize(cx,cy),QImage::Format_ARGB32);//drag시 따라올 image이다.
        image.fill(Qt::transparent);//투명한 배경을 만든다.
        QPainter painter(&image);
        painter.setRenderHint(QPainter::Antialiasing);
        QRect rect;
        QPoint origin = parent_->mapToGlobal(QPoint(0,0));
        origin = con->mapFromGlobal(origin);
        origin *= scale;
        rect.setRect(origin.x(),origin.y(),x,y);
        mapScreen->getScene()->render(&painter);//mindmapView의 scene으로부터 image를 rendering한다.
        QImage cutImage = image.copy(rect);//필요한 부분만 자른다.

        drag->setPixmap(QPixmap::fromImage(cutImage));//pixmap을 설정한다.
        if(parent_->isImageMode())//cursor의 위치를 설정한다. imageMode면 image의 왼쪽위를 잡는다.
            drag->setHotSpot((static_cast<QWidget*>(parent()))->mapFromGlobal(mapToGlobal(QPoint(0,0))));
        else
            drag->setHotSpot((static_cast<QWidget*>(parent()))->mapFromGlobal(mapToGlobal(event->pos()))*scale);
        drag->exec();
        focusOut();//drag상태에서는 focus된 node가 없다.
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

QString NodeTextEdit::labelText(){//textEdit와 label이 text를 배치하는 방법이 다르기 때문에 변환해준다.
    QString temp = "";
    for(int i=0;i<textVector_.count()-1;i++){//textVector는 label에 배치될 각 줄을 순서대로 가지고 있다.
        temp += textVector_[i] + "\n";//'\n'으로 줄을 이어주면 된다.
    }
    temp += textVector_.last();
    return temp;
}

MainWindow* NodeWidget::mainWindow = nullptr;

NodeWidget::NodeWidget(QString name){
    init();//초기화
    selfWidget.setText(name);//default text setting
    edit.saveText(name);//default text setting
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
    edit.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//textEdit에서 scrollbar가 자동으로 생기지 않게 설정.
    edit.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    PropertyDock=mainWindow->getPropertyDock();
    textViewDock=mainWindow->getTextViewDock();
    edit.setContextMenuPolicy(Qt::NoContextMenu);//textEdit의 default context menu를 사용하지 못하게 설정.
    selfWidget.setContainer(this);
    this->setStyleSheet("background-color: transparent");
    //selfWidget.setStyleSheet("background-color: transparent ; border-bottom: 1px solid black;");
    //selfWidget.setStyleSheet("border: 2px solid gray;");
    selfWidget.setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);//layout크기에 맞춰 크기 조절. layout 안에 하나의 widget만 들어있을 경우, layout이 최소크기를 잘 잡아줌.
    childWidget.setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);//y축은 layout에 맞춰 크기를 조절, x축은 layout 크기와 관계없이 내용물의 size에 맞춰 조절.
    layout.addWidget(&selfWidget);
    layout.addWidget(&childWidget);
    layout.setSpacing(30);
    this->setLayout(&layout);
    childWidget.setLayout(&childLayout);
    layout.setContentsMargins(0,0,0,0);
    childLayout.setMargin(0);

    selfWidget.setContextMenuPolicy(Qt::CustomContextMenu);//insert image action을 만들기 위해서 customContextMenu를 만듦.
    QObject::connect(&selfWidget, SIGNAL(customContextMenuRequested(const QPoint &)),this,SLOT(showCustomMenu(const QPoint&)));//우클릭시 메뉴 호출

    QFont *font = new QFont("Arial");//default font
    font->setPointSize(14);
    this->font = *font;
    fm = new QFontMetrics(*font);//textEdit의 size조절을 위해 사용. text가 배치되었을 때의 크기를 측정 가능.
    selfWidget.setFont(*font);
    edit.setFont(*font);
    textEditSizeRenew();

    pen.setWidth(2);

    QObject::connect(&edit,SIGNAL(enterPressed()),NodeWidget::mainWindow,SLOT(renewTextEdit()));//각 signal, slot항목 참조.
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

    QQueue<NodeWidget*> queue;
    NodeWidget* temp;

    if(this==getRoot()) // NodeWidget::mainWindow->getMap()
    {
        subNode->selfWidget.setDefaultColor(counter%6);
        counter++;

        queue.push_back(subNode);

        QColor* col = new QColor(subNode->selfWidget.getDefaultColorString());

        while(!queue.empty()){
            temp = queue.front();

            temp->selfWidget.setDefaultColor((counter-1)%6);
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

    else if(subNode->parent_!=nullptr)//맵이면 안됨
    {
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

NodeWidget* NodeWidget::searchFocusInNode(NodeWidget* root){//bfs로 구현.
    QQueue<NodeWidget*> queue;
    NodeWidget* temp;
    int i;

    if(root == nullptr)
        return nullptr;
    queue.push_back(root);

    while(!queue.empty()){//queue가 빌 때 까지 search
        temp = queue.front();//queue에 모두 담음
        queue.pop_front();
        if(temp->editMode){//temp가 focus되어있으면 반환하고 돌아감
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
        edit.selectAll();//모든 text가 drag된 상태로 edit가 열린다
        selfWidget.close();//label 비활성화
        delete layout.takeAt(0);//label을 layout에서 제거한 후
        layout.insertWidget(0,&edit);//edit를 layout에 추가한다
        edit.show();//edit 활성화
        edit.setFocus();
    }
}

void NodeWidget::textEditToLabel(){
    if(editMode){
        if(edit.getSavedText()!=edit.toPlainText()){//text가 바뀌었을 경우
            emit commanded(this, CommandType::Text);//undo stack에 command를 추가한다.
            edit.saveText(edit.toPlainText());//바뀐 text를 저장한다
        }

        editMode = false;
        selfWidget.show();//label활성화
        labelSizeRenew();
        delete layout.takeAt(0);//edit를 layout에서 제거한 후
        layout.insertWidget(0,&selfWidget);//label을 layout에 추가한다
        edit.close();//edit 비활성화
    }
}

void NodeWidget::closeTextEdit(){
    if(editMode){//text 변경사항을 적용하지 않고 label로 전환한다
        editMode = false;
        selfWidget.show();
        delete layout.takeAt(0);
        layout.insertWidget(0,&selfWidget);
        edit.close();
    }
}

void NodeWidget::textEditSizeRenew(){//300fixel전까지는 text width에 맞게 커지다가 300을 넘으면 크기가 고정된다.
    if(editMode){
        QString text = edit.toPlainText();
        int x = fm->width(text);//text의 width를 받는다
        int space = 11;//첫 글자와 끝 글자가 textEdit와 떨어져 있어야 하는 pixel 수이다
        int max;
        bool widthLimit;

        if(font.italic())
            space = 14;//italic체일 경우에는 space가 좀 더 크다

        QStringList enterSplit = text.split('\n');//몇 줄인지 확인한다
        QVector<QString> splitVector = enterSplit.toVector();

        if(splitVector.count()<=1){//한 줄
            if(x>300)//만약 text의 width가 300을 넘으면 크기를 제한한다
                widthLimit = true;
            else//아니면 제한하지 않는다.
                widthLimit = false;
            max = x;
        }
        else{//두 줄 이상
            int temp;

            max = fm->width(splitVector[0]);
            for(int i=1; i<splitVector.count();i++){//가장 width가 큰 줄을 찾는다
                temp = fm->width(splitVector[i]);
                if(max < temp)
                    max = temp;
            }

            if(max>300)//최대 width가 300을 넘으면 크기를 제한한다
                widthLimit = true;
            else//아니면 제한하지 않는다
                widthLimit = false;
        }

        if(widthLimit){//크기가 제한되어 있을 경우 size설정
            int y = edit.document()->size().height();
            edit.setFixedSize(300 + space,y);
        }
        else{//크기가 제한되지 않을 경우 size설정
            edit.setFixedWidth(max + space);//최대 width에 맞춰 size를 설정
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
        for(int i=0;i<charCount;i++){//char 하나씩 순차적으로 넣으면서 label의 text를 만든다. 정확히는 label의 text를 만들기 위한 textVector를 만드는데 목적이 있다
            temp += text[i];//char 하나를 넣는다
            if(text[i] == '\n'){//다음줄로 넘어가면 textVector의 원소를 하나 더 만들어야 한다.
                edit.textVector().append("");
                for(int i=0;i<temp.count()-1;i++)
                    edit.textVector()[edit.textVector().count()-1]+=temp[i];
                temp = "";
                continue;
            }
            if(fm->width(temp) > 300){//만약 문자를 넣고 났더니 한 줄의 width가 300픽셀을 넘을 경우 다음줄로 넘어가야 한다. 즉 textVector의 원소를 하나 더 만들어야 한다.
                spaceSplit = temp.split(' ');//다음줄로 넘어갈 때 space가 문장 안에 있었다면, 마지막 space 이후의 sub text는 모두 다음줄로 넘어가게 된다. 때문에 따로 처리한다.
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
    selfWidget.setText(edit.labelText());//textVector를 만들었으므로 labelText를 생성할 수 있다.
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
    minLength = diff.manhattanLength();//택시거리로 비교한다

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
    emit commanded(this,CommandType::Image);//이미지가 설정된 것도 undo할 수 있게 command를 undo stack에 넣는다.

}

void NodeWidget::imageResize(){
    int x;
    int y;
    x = image.size().width();
    y = image.size().height();
    double xFrac = x/400;//image가 400*300을 넘어가면 비율을 유지하면서 최대한 크게 이미지의 size를 설정한다.
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
    QImage scaledImage(QSize(x,y),QImage::Format_RGB32);
    scaledImage=image.scaled(QSize(x,y),Qt::KeepAspectRatio,Qt::SmoothTransformation);
    selfWidget.setPixmap(QPixmap::fromImage(scaledImage));
}

void NodeWidget::paintEvent(QPaintEvent *e){
    QPainter painter(this);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);

    QPoint pos1, pos2;

    if(editMode){//edit일 때와 아닐 때 edge를 그리기 시작하는 점이 다르므로 구분한다.
        for(int i = 0; i<child.count(); i++){
            painter.setPen(child[i]->getPen());

            QPoint temp;
            temp = edit.mapToGlobal(edit.rect().bottomRight());//this의 오른쪽 아래부터
            pos1 = this->mapFromGlobal(temp);
            temp = child[i]->selfWidget.mapToGlobal(child[i]->selfWidget.rect().bottomLeft());//child의 왼쪽 아래까지 그린다.
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
