#include "headers/parsing.h"

int getQqueue(QString& inText, QQueue<MdString>& mdQueue)
{
    QStringList list = inText.split('\n',QString::SkipEmptyParts);

    mdString *temp;

    temp = new MdString;
    int found;

    for(int i=0;i<list.size();i++)
    {
        if (list[i].at(0) == '#')
        {
            if (list[i].at(1) == '#')
            {
                if (list[i].at(2) == '#')
                {
                    found = 3;

                }
                found = 2;
            }

            found = 1;
        }
        else




    }
           /*
                        {
                            found = sentence.find_first_not_of("#");

                            temp->setDepth(found);

                            found = sentence.find_first_not_of(" \t", found);
                            temp->setText(sentence.substr(found));

                        }
                        else
                        {
                            found = sentence.find_first_of("-");

                            temp->setDepth(found + 4);

                            found = sentence.find_first_not_of(" \t", found + 1);
                            temp->setText(sentence.substr(found));
                        }
    */
        mdQueue.enqueue(temp);

    }
    return 0;
}
