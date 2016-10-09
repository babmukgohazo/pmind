#ifndef FIRSTSCREEN_H
#define FIRSTSCREEN_H
#include <QWidget>
#include <QPushButton>
#include <QLayout>

class FirstScreen:public QWidget{
public:
    FirstScreen();
    ~FirstScreen();
private:
    QPushButton *pbNew;
    QPushButton *pbLoad;
    QPushButton *pbExit;
    QHBoxLayout *layout;
};
#endif // FIRSTSCREEN_H
