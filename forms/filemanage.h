#ifndef FILEMANAGE_H
#define FILEMANAGE_H
#include <QMessageBox>
#include <QMainWindow>
#include <QTextStream>
#include <QFile>

namespace Ui {
class FileManage;
}

class FileManage : public QDialog
{
    Q_OBJECT
public:
    explicit FileManage(QWidget *parent = 0);
    ~FileManage();

signals:
    void signal_new();
    void signal_open();
public slots:
    void on_newButton_clicked(); //new 버튼 눌렸을 때
    void on_openButton_clicked();   //open 버튼 눌렀을 때
    void on_quitButton_clicked();  //quit 버튼 눌렀을 때

private:
    Ui::FileManage *ui;
};

#endif // FILEMANAGE_H
