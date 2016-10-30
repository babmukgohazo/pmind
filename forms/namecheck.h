#ifndef NAMECHECK_H
#define NAMECHECK_H

#include <QDialog>

namespace Ui {
class nameCheck;
}

class nameCheck : public QDialog
{
    Q_OBJECT

public:
    explicit nameCheck(QWidget *parent = 0);
    ~nameCheck();

private:
    Ui::nameCheck *ui;
};

#endif // NAMECHECK_H
