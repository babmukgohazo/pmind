#include "headers/mainwindow.h"

MindmapView::MindmapView(){
    mindmapScene = new QGraphicsScene();
    this->setScene(mindmapScene);
    setDragMode(QGraphicsView::ScrollHandDrag);
    currentScale = 100;
    editClicked = false;
    labelClicked = false;
    //this->setBackgroundBrush(QBrush(QColor(250,250,250), Qt::SolidPattern));

    //184 184 184
    //242 242 242
}

void MindmapView::mousePressEvent(QMouseEvent *e){
    update();
    QGraphicsView::mousePressEvent(e);
    focusIn();
}

void MindmapView::keyPressEvent(QKeyEvent *e){
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
