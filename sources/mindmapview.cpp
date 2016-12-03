#include "headers/mainwindow.h"

MindmapView::MindmapView(){
    mindmapScene = new QGraphicsScene();
    this->setScene(mindmapScene);
    setDragMode(QGraphicsView::ScrollHandDrag);
    currentScale = 100;
    editClicked = false;
    labelClicked = false;
}

void MindmapView::mousePressEvent(QMouseEvent *e){
    update();
    QGraphicsView::mousePressEvent(e);
    focusIn();
}

void MindmapView::keyPressEvent(QKeyEvent *e){
    bool modifier = true;
    switch(e->key()){
    case Qt::Key_Z:
        if(e->modifiers().testFlag(Qt::ControlModifier))
            emit undid();
        else
            modifier = false;
        break;
    case Qt::Key_Y:
        if(e->modifiers().testFlag(Qt::ControlModifier))
            emit redid();
        else
            modifier = false;
        break;
    case Qt::Key_N:
        if(e->modifiers().testFlag(Qt::ControlModifier))
            emit newFile();
        else
            modifier = false;
        break;
    case Qt::Key_S:
        if(e->modifiers().testFlag(Qt::ControlModifier))
            emit save();
        else if(e->modifiers().testFlag(Qt::ShiftModifier))
            emit saveAs();
        else
            modifier = false;
        break;
    case Qt::Key_L:
        if(e->modifiers().testFlag(Qt::ControlModifier))
            emit load();
        else
            modifier = false;
        break;
    case Qt::Key_Q:
        if(e->modifiers().testFlag(Qt::ControlModifier))
            emit quit();
        else
            modifier = false;
        break;
    case Qt::Key_E:
        if(e->modifiers().testFlag(Qt::ControlModifier))
            emit imageExport();
        else
            modifier = false;
        break;
    default:
        modifier = false;
        break;
    }
    if(!modifier)
        QGraphicsView::keyPressEvent(e);
}

void MindmapView::focusIn(){
    NodeWidget* temp;
    temp = NodeWidget::searchFocusInNode(mainWindow->getMap());
    if(temp != nullptr){
        if(!labelClicked){
            temp->label().focusOut();
            if(!editClicked)
                temp->textEditToLabel();
        }
    }
    editClicked = false;
    labelClicked = false;
    this->setFocus();
}
