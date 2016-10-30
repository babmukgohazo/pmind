#ifndef SIGMOIDCURVE_H
#define SIGMOIDCURVE_H

#include "mainwindow.h"
#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QPointF>
#include <QPolygonF>
#include <QMouseEvent>
#include <cmath>
#include "tree.h"
#define PI 3.141592
#define SIG5 0.986614298

class SigmoidCurve : public QWidget {
private://노드를 저장해놓는다.
    struct Curve {
        QPolygonF poly;
        QPointF center;
        QPointF initial;
        QPointF final;
        double curveHeight;
        double curveWidth;
        double curveLength;
        int totalPoints;
        double xInterval;
		double sin;
		double cos;
    };
    Curve* curve;
    QPointF initial;
    QVector<QPointF> final;
    int polyCount;
	double width = 2;//두께 변화 정도
	double limit = 1;//최소 두께

public:
    SigmoidCurve();
    SigmoidCurve(QPointF initial, QVector<QPointF> final);
    SigmoidCurve(QPointF initial, QVector<QPointF> final, QWidget* parent);

	void addFinalPoint(QPointF temp);//곡선 추가
	void addFinalPoint(int finalX, int finalY);//곡선 추가
	void wholeCurveInitialization();//모든 곡선 다시 생성 후 초기화
	void curveInitialization(Curve* curve, QPointF initial, QPointF final);//곡선 한 개 초기화
	double sigmoid(Curve *curve, double x);//곡선 함수
	QPointF lineWidth(Curve* curve, double x, double y);//곡선의 두께를 결정
	void drawSigmoidPolygon(Curve *curve);//polygon 한 개 그림
	void drawAllPolygon();//모든 polygon 그림

protected://마우스 이벤트 test
//    void mouseMoveEvent(QMouseEvent *event){
//        final[0] = event->pos();
//        update();
//    }
    void paintEvent(QPaintEvent* event) {
        if (polyCount != 0)
            delete[] curve;
        wholeCurveInitialization();
        drawAllPolygon();
    }
};



#endif // SIGMOIDCURVE_H
