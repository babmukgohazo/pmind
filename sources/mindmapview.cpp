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
    emit viewClicked();
    QGraphicsView::mousePressEvent(e);
    focusIn();
}

void MindmapView::keyPressEvent(QKeyEvent *e){
    switch(e->key()){
    case Qt::Key_Z:
        if(e->modifiers().testFlag(Qt::ControlModifier))
            emit undid();
        else
            QGraphicsView::keyPressEvent(e);

        break;
    case Qt::Key_Y:
        if(e->modifiers().testFlag(Qt::ControlModifier))
            emit redid();
        else
            QGraphicsView::keyPressEvent(e);
        break;
    default:
        QGraphicsView::keyPressEvent(e);
        break;
    }
}

void MindmapView::focusIn(){
    NodeWidget* temp;
    temp = NodeWidget::searchFocusInNode(mainWindow->getMap());
    if(temp != nullptr){
        if(!labelClicked){
            temp->label().focusOut();
            if(editClicked)
                temp->textEditToLabel();
        }
    }
    editClicked = false;
    labelClicked = false;
    this->setFocus();
}
