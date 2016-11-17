#include "propertytab.h"
#include "ui_propertytab.h"

PropertyTab::PropertyTab(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::PropertyTab)
{
    ui->setupUi(this);
}

QTextEdit* PropertyTab::getTextEdit()
{
    return ui->textEdit;
}

PropertyTab::~PropertyTab()
{
    delete ui;
}
