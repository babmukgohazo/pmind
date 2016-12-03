#include "textview.h"
#include "ui_textview.h"

TextView::TextView(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::TextView)
{
    ui->setupUi(this);
}

TextView::~TextView()
{
    delete ui;
}

QTextEdit* TextView::getTextEdit() {
    return ui->textEdit;
}
