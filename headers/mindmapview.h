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

    void adjustScale(qreal ratio){
        qreal x,y;
        x = ratio/currentScale;
        y = ratio/currentScale;
        scale(x,y);
        currentScale = ratio;
    }

    double getCurrentScale(){
        return currentScale;
    }

    QGraphicsScene* getScene(){return mindmapScene;}

    void wheelEvent(QWheelEvent *event){
        event->delta() > 0 ? zoomIn() : zoomOut();
    }
    void mousePressEvent(QMouseEvent *e);
    void keyPressEvent(QKeyEvent *e);

signals:
    void zoomSignal();

public slots:

    void zoomIn(){
        qreal scale_ = currentScale;
        scale_ *= 11.0/10;
        if(scale_>300)
            scale_ = 300;
        adjustScale(scale_);
        emit zoomSignal();
    }
    void zoomOut(){
        qreal scale_ = currentScale;
        scale_ *= 10.0/11;
        if(scale_<50)
            scale_ = 50;
        adjustScale(scale_);
        emit zoomSignal();
    }

    void focusIn();
    void editClick(){editClicked = true;}
    void labelClick(){labelClicked = true;}

private:
    double currentScale;
    bool editClicked;
    bool labelClicked;
};

#endif
