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

    void adjustScale(int ratio){
        qreal x,y;
        x = ratio/currentScale;
        y = ratio/currentScale;
        scale(x,y);
        currentScale = ratio;
    }

    double getCurrentScale(){
        return currentScale;
    }

signals:
    void zoomSignal();

public slots:

    void zoomIn(){
        scale(1.25,1.25);
        currentScale *=1.25;
        emit zoomSignal();
    }
    void zoomOut(){
        scale(0.8,0.8);
        currentScale *=0.8;
        emit zoomSignal();
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

private:
    double currentScale;
};

#endif
