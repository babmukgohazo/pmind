#ifndef EDITSCREEN_H
#define EDITSCREEN_H
#include <QLabel>
#include <QBoxLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QGraphicsView>
#include "headers/mindmapview.h"
#include "headers/nodewidget.h"
#include "headers/parsing.h"

class EditScreen:public QWidget{
    Q_OBJECT
public:
    EditScreen();
    ~EditScreen();
public slots:
    void reload(){
        if (map!=nullptr)
            delete map;
        QString str = edit->toPlainText();
        QQueue<MdString> q;
        getQqueue(str,q);
        map = new NodeWidget(q);
        mapScreen->mindmapScene->addWidget(map);
    }
private:
    MindmapView* mapScreen;
    NodeWidget* map;
    QTextEdit* edit;
    QPushButton* redrawButton;
    QHBoxLayout* layout;
    QVBoxLayout* rightLayout;
};

#endif // EDITSCREEN_H
