#ifndef MINDMAPVIEW_H
#define MINDMAPVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include "headers/nodewidget.h"


class MindmapView : public QGraphicsView{
    Q_OBJECT

public:
    MindmapView(QWidget* centralWidget): QGraphicsView(centralWidget){

        mindmapScene = new QGraphicsScene();
        this->setScene(mindmapScene);
    }
    QGraphicsScene* mindmapScene;
public slots:

    void zoomIn(){
        scale(1.2,1.2);
    }
    void zoomOut(){
        scale(0.8,0.8);
    }

protected:
    void wheelEvent(QWheelEvent *event){
        event->delta() > 0 ? zoomIn() : zoomOut();
    }
};

#endif
