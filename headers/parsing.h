#ifndef PARSING_H
#define PARSING_H
#include<QString>
#include<QQueue>
#include<QStringList>

class MdString{

public:
    int getDepth();
    void setDepth(int);
    int getText();
    void setText(QString);

private:
    int depth;
    QString text;

};

int getQqueue(QString&,QQueue&);

#endif // PARSING_H
