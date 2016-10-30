#include "headers/editscreen.h"

EditScreen::EditScreen(){
    mapScreen = new MindmapView();
    edit = new QTextEdit();
    redrawButton = new QPushButton("Redraw");
    layout = new QHBoxLayout();
    rightLayout = new QVBoxLayout();
    map = new NodeWidget();

    rightLayout->addWidget(edit);
    rightLayout->addWidget(redrawButton);
    layout->addWidget(mapScreen);
    layout->addLayout(rightLayout);
    layout->setStretchFactor(mapScreen,7);
    layout->setStretchFactor(rightLayout,3);

    mapScreen->setObjectName("mapscreen");
    mapScreen->setStyleSheet("#mapscreen {border: 1px solid gray; background: white;}");
    this->setLayout(layout);

    QObject::connect(redrawButton, SIGNAL(clicked()),this,SLOT(reload()));
}
EditScreen::~EditScreen(){
    delete redrawButton;
    delete rightLayout;
    delete edit;
    delete layout;
    delete mapScreen;
}
