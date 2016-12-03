#include "headers/parsing.h"
#include "headers/nodewidget.h"

int getQqueue(QString &inText, QQueue<MdString> &mdQueue)
{
    QStringList list = inText.split('\n',QString::SkipEmptyParts);
    mdQueue.clear();
    MdString *temp;

    int found;

    for(int i=0;i<list.size();i++)
    {
        found = 0;

        temp = new MdString;

        if (list[i].at(0) == '#')
        {
            if (list[i].at(1) == '#')
            {
                if (list[i].at(2) == '#')
                {
                    found++;

                }
                found++;

            }

            found++;


            temp->setDepth(found-1);
            temp->setText(list[i].mid(found+1));

        }
        else if (list[i].at(0) == '-')
        {
            temp->setDepth(found+3);
            temp->setText(list[i].mid(found+2));

        }
        else if (list[i].at(0) == '\t')
        {

            for(int j=0;list[i].at(j)=='\t';j++)
            {
                found++;
            }

            if(list[i].at(found) != '-')
            {
                return -1;
            }

            temp->setDepth(found+3);
            temp->setText(list[i].mid(found+2));

        }
        else
        {
            return -1;
        }

        mdQueue.enqueue(*temp);

    }

    return 0;
}

int convertQqToText(QString &inText,QQueue<MdString> &mdQueue){
    inText = "";

    for (int i = 0; i < mdQueue.length(); i++){
        MdString temp = mdQueue.at(i);
        int depth = temp.getDepth();
        if(depth == 0){
            inText.append("# "+temp.getText()+"\n\n");
        }
        else if(depth == 1){
            inText.append("## "+temp.getText()+"\n\n");
        }
        else if(depth == 2){
            inText.append("### "+temp.getText()+"\n\n");
        }
        else if(depth == 3){
            inText.append("- "+temp.getText()+"\n\n");
        }
        else if(depth >= 4){
            for (int i = depth; i > 3; i--){
                inText.append("\t");
            }
            inText.append("- "+temp.getText()+"\n\n");
        }
    }
}

void dfs(QStack<NodeWidget*>& qStack, QQueue<MdString>& mdQueue){

    NodeWidget* tempNode = qStack.pop();
    MdString temp;
    temp.setDepth(tempNode->getDepth());
    temp.setText(tempNode->label().text());
    mdQueue.enqueue(temp);

    QVector<NodeWidget*>& childNodes = tempNode->getChild();

    for (int i = childNodes.length()-1; i >= 0; i--){
        qStack.push(childNodes.at(i));
        dfs(qStack, mdQueue);
    }
}

int MdString::getDepth()
{
    return depth;
}

QString MdString::getText()
{
    return text;
}

void MdString::setDepth(int dep)
{
    depth = dep;
}

void MdString::setText(QString str)
{
    text = str;
}
