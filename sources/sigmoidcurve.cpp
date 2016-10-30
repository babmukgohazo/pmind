#include "headers/sigmoidcurve.h"


SigmoidCurve::SigmoidCurve() {
    curve = '\0';
    initial = QPointF(0, 0);
    polyCount = 0;
}
SigmoidCurve::SigmoidCurve(QPointF initial, QVector<QPointF> final) {//노드를 받는다. 그리고 하위 노드의 개수를 polyCount로 설정한다.

    if(final.isEmpty())
        return;
    this->initial = initial;
    this->final = final;
    polyCount = final.count();
    wholeCurveInitialization();
    resize(final.last().x(), final.last().y());
}

SigmoidCurve::SigmoidCurve(QPointF initial, QVector<QPointF> final, QWidget* parent):QWidget(parent){
    if(final.isEmpty())
        return;
    this->initial = initial;
    this->final = final;
    polyCount = final.count();
    wholeCurveInitialization();
    resize(final.last().x(), final.last().y());
}

void SigmoidCurve::addFinalPoint(QPointF temp) {
    final.append(temp);
    polyCount = final.count();
    wholeCurveInitialization();
}
void SigmoidCurve::addFinalPoint(int finalX, int finalY) {
    final.append(QPointF(finalX, finalY));
    polyCount = final.count();
    wholeCurveInitialization();
}
void SigmoidCurve::wholeCurveInitialization() {
    curve = new Curve[polyCount];
    for (int i = 0;i<polyCount;i++) {
        curveInitialization(&curve[i], initial, final[i]);
    }
}
void SigmoidCurve::curveInitialization(Curve* curve, QPointF initial, QPointF final) {
    QPolygon newPoly;
    curve->poly = newPoly;
    curve->initial = initial;
    curve->final = final;
    curve->center = (curve->initial + curve->final) / 2;
    curve->curveHeight = curve->final.y() - curve->initial.y();
    curve->curveWidth = curve->final.x() - curve->initial.x();
    curve->curveLength = sqrt(pow(curve->curveHeight, 2) + pow(curve->curveWidth, 2));
    if (curve->curveWidth > 200)
        curve->totalPoints = fabs(curve->curveHeight) / 10 + curve->curveWidth / 40;
    else
        curve->totalPoints = fabs(curve->curveHeight) / 10 + 5;
    curve->xInterval = curve->curveWidth / curve->totalPoints;
    curve->sin = curve->curveHeight / curve->curveLength;
    curve->cos = curve->curveWidth / curve->curveLength;
}
double SigmoidCurve::sigmoid(Curve *curve, double x) {
    double halfWidth = curve->curveWidth / 2;
    return curve->curveHeight / (1 + exp(-x * 5 / halfWidth)) / SIG5;
}
QPointF SigmoidCurve::lineWidth(Curve* curve, double x, double y) {
    double length = sqrt(pow(x, 2) + pow(y, 2));
    if (curve->curveLength == 0)
        return QPointF(0, 0);
    double convertX = -curve->sin*(width*(1 - length / curve->curveLength) + limit);
    double convertY = curve->cos*(width*(1 - length / curve->curveLength) + limit);
    return QPointF(convertX, convertY);
}
void SigmoidCurve::drawSigmoidPolygon(Curve *curve) {//특정 하위 노드와의 sigmoid polygon을 만든다. 배치가 달라진 노드만 새로 polygon을 만들면 좋을 듯 싶다.
    QPainter *painter = new QPainter(this);
    QPen pen;
    int i;
    double x, y, halfWidth;
    pen.setWidthF(0);
    painter->setPen(pen);
    painter->setBrush(Qt::black);
    painter->setRenderHint(QPainter::Antialiasing);
    if (curve->curveWidth != 0) {
        for (i = 0;i <= curve->totalPoints;i++) {
            halfWidth = curve->curveWidth / 2;
            x = curve->xInterval*i;
            y = sigmoid(curve, x - halfWidth) - sigmoid(curve, -halfWidth);
            curve->poly << curve->initial + QPointF(x, y) - lineWidth(curve, x, y);
        }
        for (i = curve->totalPoints;i >= 0;i--) {
            halfWidth = curve->curveWidth / 2;
            x = curve->xInterval*i;
            y = sigmoid(curve, x - halfWidth) - sigmoid(curve, -halfWidth);
            curve->poly << curve->initial + QPointF(x, y) + lineWidth(curve, x, y);
        }
    }
    else {
        for (i = 0;i <= curve->totalPoints;i++)
            curve->poly << curve->initial + QPointF(0, curve->curveHeight*i / curve->totalPoints);
        for (i = curve->totalPoints;i >= 0;i--)
            curve->poly << curve->initial + QPointF(0, curve->curveHeight*i / curve->totalPoints);
    }
    painter->drawPolygon(curve->poly);
    painter->drawEllipse(curve->initial, width + limit, width + limit);
}
void SigmoidCurve::drawAllPolygon() {
    for (int i = 0;i<polyCount;i++)
        drawSigmoidPolygon(&curve[i]);
}
