#include "headers/nodewidget.h"


NodeWidget::NodeWidget(QString name){
    this->setStyleSheet("background-color: white");
    selfWidget.setText(name);
    selfWidget.setStyleSheet("border: 2px solid gray;");
    selfWidget.setSizePolicy(QSizePolicy::QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    layout.addWidget(&selfWidget);
    layout.addWidget(&childWidget);
    layout.setSpacing(30);
    this->setLayout(&layout);
    childWidget.setLayout(&childLayout);
    layout.setContentsMargins(0,0,0,0);
    childLayout.setMargin(0);
}

NodeWidget::NodeWidget(QQueue<MdString> list){
    this->setStyleSheet("background-color: white");
    selfWidget.setStyleSheet("border: 2px solid gray;");
    selfWidget.setSizePolicy(QSizePolicy::QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    layout.addWidget(&selfWidget);
    layout.addWidget(&childWidget);
    layout.setSpacing(30);
    this->setLayout(&layout);
    childWidget.setLayout(&childLayout);
    layout.setContentsMargins(0,0,0,0);
    childLayout.setMargin(0);

    if(list.isEmpty()){
        selfWidget.setText("ERROR: not valid .md file");
        return;
    }
    else{
        selfWidget.setText(list.dequeue().getText());
    }


    NodeWidget* ptr = this;
    int currentDepth = 0;
    while(!list.isEmpty()){
        int temp = list.first().getDepth();
        if(temp == currentDepth+1){
            MdString tempMdString = list.dequeue();
            temp = tempMdString.getDepth();
            NodeWidget* tempnode = new NodeWidget(tempMdString.getText());
            ptr->add(tempnode);
            ptr = tempnode;
            currentDepth++;
        }
        else{
            ptr = ptr->getParent();
            currentDepth--;
        }
    }
}


NodeWidget* NodeWidget::getChild(int i){
    return child[i];
}

void NodeWidget::add(NodeWidget *subNodeWidget){
    child.push_back(subNodeWidget);
    childLayout.addWidget(child[childNum]);
    child[childNum]->parent_ = this;
    childNum++;
}

void NodeWidget::paintEvent(QPaintEvent *e){
    QPainter painter(this);
    QPen myPen;
    myPen.setWidth(1);
    painter.setPen(myPen);

    painter.drawLine(selfWidget.mapToParent(selfWidget.rect().bottomLeft()),selfWidget.mapToParent(selfWidget.rect().bottomRight()));
    for(int i = 0 ; i<childNum ; i++){
        QPoint temp;
        temp = selfWidget.mapToGlobal(selfWidget.rect().bottomRight());
        QPoint pos1 = this->mapFromGlobal(temp);
        temp = child[i]->selfWidget.mapToGlobal(child[i]->selfWidget.rect().bottomLeft());
        QPoint pos2 = this->mapFromGlobal(temp);


        QPainterPath path;
        path.moveTo(pos1);
        path.cubicTo(pos1+QPoint(10,0),pos2+QPoint(-10,0),pos2);
        painter.drawPath(path);
    }
}
