#ifndef XMLHANDLER_H
#define XMLHANDLER_H
#include <QString>
#include <QtXml>
#include "nodewidget.h"

class XmlHandler{

public:
//    static QString mindmap2Xml(NodeWidget* map){
//        QDomDocument doc("Pmind");
//        QDomElement root = doc.createElement("Pmind");
//        doc.appendChild(root);
//        QDomElement mapElement = doc.createElement("map");
//        mapElement.appendChild(createNodeElement(doc, map));
//        root.appendChild(mapElement);

//        int indent = 0;
//        return doc.toString(indent);
//    }
    static QString mindmap2Xml(NodeWidget* map){
        QDomDocument doc("Pmind");
        QDomElement root = doc.createElement("Pmind");
        doc.appendChild(root);
        QDomElement mapElement = doc.createElement("map");
        createNodeElement(doc,mapElement, map);
        root.appendChild(mapElement);

        int indent = 0;
        return doc.toString(indent);
    }
//    static NodeWidget* Xml2Mindmap(QDomDocument doc){
//        QDomElement head = doc.elementsByTagName("map").at(0).childNodes().at(0).toElement();
//        return createNodeWidget(head);
//    }
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
            //headptr->label().setNodeTextColor(head.attribute("textcolor").toInt());
            headptr->label().setNodeTextColor(QColor(head.attribute("textcolor").right(8).left(7)));
            headptr->label().setDefaultColor(head.attribute("defaultcolor").toInt());

            QDomNodeList list = mapElement.childNodes();
            NodeWidget* ptr = headptr;
            int currentDepth = 0;

            for(int i = 1 ; i<list.size();){
                QDomElement e = list.at(i).toElement();
                int temp = e.attribute("depth").toInt();
                if(temp == currentDepth+1){

                    //NodeWidget* newNode = new NodeWidget(e.attribute("content").toUtf8());
                    NodeWidget* newNode = new NodeWidget(QString::fromUtf8(e.attribute("content").toUtf8()));
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
//    static QDomElement createNodeElement(QDomDocument& doc, NodeWidget* node){
//        QDomElement e = doc.createElement("node");
//        e.setAttribute("content", node->label().text());
//        e.setAttribute("nodestyle", node->label().getNodeShape());
//        e.setAttribute("font", node->label().font().toString());
//        e.setAttribute("defaultcolor", node->label().getDefaultColor());
//        e.setAttribute("textcolor", node->label().getNodeTextColor());
//        e.setAttribute("depth", node->getDepth());

//        for(int i = 0 ; i< node->getChild().size(); i++){
//            e.appendChild(createNodeElement(doc, mapElement, node->getChild().at(i)));
//        }

//        return e;
//    }
    static QDomElement createNodeElement(QDomDocument& doc, QDomElement& mapElement, NodeWidget* node){
        QDomElement e = doc.createElement("node");
        //node->label().text().toUtf8();
        e.setAttribute("content", QString(node->label().text().toUtf8()));
        //e.setAttribute("content", node->label().text());
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
    static NodeWidget* createNodeWidget(QDomElement e){
        //NodeWidget* newNode = new NodeWidget();
        //newNode->label().setText(e.attribute("content"));

        NodeWidget* newNode = new NodeWidget(e.attribute("content"));
        newNode->label().setNodeShape(e.attribute("nodestyle").toInt());
        QFont font;
        font.fromString( e.attribute("font"));
        newNode->label().setFont(font);

        newNode->label().setNodeTextColor(e.attribute("textcolor").toInt());

        newNode->label().setDefaultColor(e.attribute("defaultcolor").toInt());

//        QString underlineCSS =  "border-top-style: none; border-right-style: none; border-bottom-style: solid; border-left-style: none; border-width: 2px;border-color: black;";
//        QString recCSS = "border-width: 2px;border-style : solid;border-color: black;";
//        QString roundRecCSS = "border-width: 2px; border-style : solid; border-radius: 4px; border-color: black;";
//        QString nothingCSS = "border: 2px solid gray;";
//        switch(e.attribute("nodestyle").toInt()){
//        case 0:
//            newNode->label().setStyleSheet(nothingCSS);
//            break;
//        case 1:
//            newNode->label().setStyleSheet(recCSS);
//            break;
//        case 2:
//            newNode->label().setStyleSheet(underlineCSS);
//            break;
//        case 3:
//            newNode->label().setStyleSheet(roundRecCSS);
//            break;

//        }

        for(int i=0; i< e.childNodes().size();i++){
            newNode->add(createNodeWidget(e.childNodes().at(i).toElement()));
            newNode->getChild().at(i)->label().setDefaultColor(e.attribute("defaultcolor").toInt());
        }

        return newNode;
    }
};

#endif // XMLHANDLER_H
