#ifndef XMLHANDLER_H
#define XMLHANDLER_H
#include <QtXml>
#include "nodewidget.h"

class XmlHandler{
public:
    static QString mindmap2Xml(NodeWidget* map){
        QDomDocument doc("Pmind");
        QDomElement root = doc.createElement("Pmind");
        doc.appendChild(root);
        QDomElement mapElement = doc.createElement("map");
        createNodeElement(doc,mapElement, map);
        root.appendChild(mapElement);

        int indent = 4;
        return doc.toString(indent);
    }

    static NodeWidget* Xml2Mindmap(QDomDocument doc){
        NodeWidget::setCounter();
        QDomElement head = doc.elementsByTagName("map").at(0).childNodes().at(0).toElement();
        QDomElement mapElement = doc.elementsByTagName("map").at(0).toElement();
        NodeWidget* headptr;

        if(head.isNull()){
            headptr = new NodeWidget("ERROR: not valid file");
        }
        else{
            headptr = new NodeWidget(head.attribute("content"));
            headptr->label().setNodeShape(head.attribute("nodestyle").toInt());
            QFont font;
            font.fromString(head.attribute("font"));
            headptr->label().setFont(font);
            headptr->label().setNodeTextColor(QColor(head.attribute("textcolor").right(8).left(7)));
            headptr->label().setDefaultColor(head.attribute("defaultcolor").toInt());

            QDomNodeList list = mapElement.childNodes();
            NodeWidget* ptr = headptr;
            int currentDepth = 0;

            for(int i = 1 ; i<list.size();){
                QDomElement e = list.at(i).toElement();
                int temp = e.attribute("depth").toInt();
                if(temp == currentDepth+1){

                    NodeWidget* newNode = new NodeWidget(e.attribute("content"));
                    newNode->label().setNodeShape(e.attribute("nodestyle").toInt());
                    QFont font;
                    font.fromString( e.attribute("font"));
                    newNode->label().setFont(font);
                    newNode->label().setNodeTextColor(QColor(e.attribute("textcolor").right(8).left(7)));
                    newNode->label().setDefaultColor(e.attribute("defaultcolor").toInt());

                    ptr->add(newNode);
                    ptr = newNode;
                    currentDepth++;
                    i++;
                }
                else{
                    ptr = ptr->getParent();
                    currentDepth--;
                }
            }
        }
        return headptr;
    }

private:
    static QDomElement createNodeElement(QDomDocument& doc, QDomElement& mapElement, NodeWidget* node){
        QDomElement e = doc.createElement("node");
        e.setAttribute("content", QString(node->label().text().toLatin1()));
        e.setAttribute("nodestyle", node->label().getNodeShape());
        e.setAttribute("font", node->label().font().toString());
        e.setAttribute("defaultcolor", node->label().getDefaultColor());
        e.setAttribute("textcolor", node->label().getNodeTextColor());
        e.setAttribute("depth", node->getDepth());
        mapElement.appendChild(e);

        for(int i = 0 ; i< node->getChild().size(); i++){
            createNodeElement(doc, mapElement, node->getChild().at(i));
        }

        return e;
    }
};

#endif // XMLHANDLER_H
