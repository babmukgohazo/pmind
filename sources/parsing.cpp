#include "headers/parsing.h"
#include <QDebug>

int getQqueue(QString &inText, QQueue<MdString> &mdQueue)
{
    QStringList list = inText.split('\n',QString::SkipEmptyParts);

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
