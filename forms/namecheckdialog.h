#ifndef NAMECHECKDIALOG_H
#define NAMECHECKDIALOG_H


#include "filemanage.h"

namespace Ui {
class NameCheckDialog;
}

class NameCheckDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NameCheckDialog(QWidget *parent = 0);
    QString fileName;
    ~NameCheckDialog();
    Ui::NameCheckDialog *ui;
signals:
    void okButtonClicked();

private slots:
    void on_okButton_clicked();

    void on_cancelButton_clicked();

private:

};

#endif // NAMECHECKDIALOG_H
