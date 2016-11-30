#include "headers/mainwindow.h"

MindmapView::MindmapView(){
    mindmapScene = new QGraphicsScene();
    this->setScene(mindmapScene);
    setDragMode(QGraphicsView::ScrollHandDrag);
    currentScale = 100;
}

void MindmapView::mousePressEvent(QMouseEvent *e){
    focusIn();
    emit viewClicked();

    QGraphicsView::mousePressEvent(e);
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
    if(temp != nullptr)
        temp->label().focusOut();
    this->setFocus();
}
