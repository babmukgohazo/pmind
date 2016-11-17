#ifndef PROPERTYTAB_H
#define PROPERTYTAB_H

#include <QDockWidget>
#include <QTextEdit>

namespace Ui {
class PropertyTab;
}

class PropertyTab : public QDockWidget
{
    Q_OBJECT

public:
    explicit PropertyTab(QWidget *parent = 0);
    QTextEdit* getTextEdit();
   // void setNodeWidget(NodeWidget* map) {this->map = map;};
    ~PropertyTab();

private:
    Ui::PropertyTab *ui;
    //NodeWidget* map;
};

#endif // PROPERTYTAB_H
