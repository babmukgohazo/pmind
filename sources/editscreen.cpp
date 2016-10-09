#include "headers/editscreen.h"

EditScreen::EditScreen(){
    mapScreen = new QWidget();
    edit = new QTextEdit();
    redrawButton = new QPushButton("Redraw");
    layout = new QHBoxLayout();
    right = new QVBoxLayout();

    right->addWidget(edit);
    right->addWidget(redrawButton);
    layout->addWidget(mapScreen);
    layout->addLayout(right);
    layout->setStretchFactor(mapScreen,7);
    layout->setStretchFactor(right,3);

    this->setLayout(layout);
}
EditScreen::~EditScreen(){
    delete mapScreen;
    delete edit;
    delete redrawButton;
    delete layout;
    delete right;
}
