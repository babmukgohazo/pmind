#ifndef EDITSCREEN_H
#define EDITSCREEN_H
#include <QLabel>
#include <QBoxLayout>
#include <QTextEdit>
#include <QPushButton>

class EditScreen:public QWidget{
public:
    EditScreen();
    ~EditScreen();
private:
    QWidget *mapScreen;
    QTextEdit *edit;
    QPushButton *redrawButton;
    QHBoxLayout *layout;
    QVBoxLayout *right;
};

#endif // EDITSCREEN_H
