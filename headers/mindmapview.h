#ifndef MINDMAPVIEW_H
#define MINDMAPVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include "headers/nodewidget.h"

class MainWindow;

class MindmapView : public QGraphicsView{
    Q_OBJECT

public:
    MindmapView();
    QGraphicsScene* mindmapScene;
    MainWindow* mainWindow;

public slots:

    void zoomIn(){
        scale(1.2,1.2);
    }
    void zoomOut(){
        scale(0.8,0.8);
    }

    void focusIn();

signals:
    void viewClicked();
    void undid();
    void redid();

protected:
    void wheelEvent(QWheelEvent *event){
        event->delta() > 0 ? zoomIn() : zoomOut();
    }
    void mousePressEvent(QMouseEvent *e);
    void keyPressEvent(QKeyEvent *e);
};

#endif
