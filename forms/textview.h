#ifndef TEXTVIEW_H
#define TEXTVIEW_H

#include <QDockWidget>
#include <QTextEdit>

namespace Ui {
class TextView;
}

class TextView : public QDockWidget
{
    Q_OBJECT

public:
    explicit TextView(QWidget *parent = 0);
    ~TextView();
    QTextEdit* getTextEdit();

private:
    Ui::TextView *ui;
};

#endif // TEXTVIEW_H
