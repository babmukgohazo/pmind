#include "headers/mindmapview.h"
#include "headers/mainwindow.h"

MindmapView::MindmapView(){
    mindmapScene = new QGraphicsScene();
    this->setScene(mindmapScene);
    setDragMode(QGraphicsView::ScrollHandDrag);
}

void MindmapView::mousePressEvent(QMouseEvent *e){

    NodeWidget* temp;
    temp = NodeWidget::searchFocusInNode(mainWindow->getMap());
    if(temp != nullptr)
        temp->label().focusOut();
    this->setFocus();
    emit viewClicked();

    QGraphicsView::mousePressEvent(e);
}
