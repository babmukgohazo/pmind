#include "headers/firstscreen.h"

FirstScreen::FirstScreen(){
    pbNew = new QPushButton("New",this);
    pbLoad = new QPushButton("Load",this);
    pbExit = new QPushButton("Exit",this);

    pbNew->setMaximumWidth(300);
    pbLoad->setMaximumWidth(300);
    pbExit->setMaximumWidth(300);


    pbNew->setMinimumHeight(200);
    pbLoad->setMinimumHeight(200);
    pbExit->setMinimumHeight(200);

    layout = new QHBoxLayout();
    layout->addWidget(pbNew);
    layout->addWidget(pbLoad);
    layout->addWidget(pbExit);
    this->setLayout(layout);
}
FirstScreen::~FirstScreen(){
    delete pbNew;
    delete pbLoad;
    delete pbExit;
    delete layout;
}
