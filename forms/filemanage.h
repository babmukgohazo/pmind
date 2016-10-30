#ifndef FILEMANAGE_H
#define FILEMANAGE_H

#include <QWidget>

namespace Ui {
class fileManage;
}

class fileManage : public QWidget
{
    Q_OBJECT

public:
    explicit fileManage(QWidget *parent = 0);
    ~fileManage();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_newButton_clicked();

    void on_openButton_clicked();

    void on_quitButton_clicked();

private:
    Ui::fileManage *ui;
};

#endif // FILEMANAGE_H
