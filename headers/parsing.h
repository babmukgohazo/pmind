#ifndef PARSING_H
#define PARSING_H
#include<QString>
#include<QQueue>
#include<QStack>
#include<QStringList>

class NodeWidget;
class MdString{

public:
    int getDepth();
    void setDepth(int);
    QString getText();
    void setText(QString);

private:
    int depth;
    QString text;

};

int getQqueue(QString &inText,QQueue<MdString> &mdQueue);
int convertQqToText(QString &inText,QQueue<MdString> &mdQueue);
void dfs(QStack<NodeWidget*>& qStack, QQueue<MdString>& mdQueue);

#endif // PARSING_H
