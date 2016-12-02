#ifndef XMLHANDLER_H
#define XMLHANDLER_H
#include <QString>
#include <QtXml>
#include "nodewidget.h"

class XmlHandler{

public:
    static QString mindmap2Xml(NodeWidget* map){
        QDomDocument doc("Pmind");
        QDomElement root = doc.createElement("Pmind");
        doc.appendChild(root);
        QDomElement mapElement = doc.createElement("map");
        mapElement.appendChild(createNodeElement(doc, map));
        root.appendChild(mapElement);

        int indent = 4;
        return doc.toString(indent);
    }
    static NodeWidget* Xml2Mindmap(QDomDocument doc){
        QDomElement head = doc.elementsByTagName("map").at(0).childNodes().at(0).toElement();
        return createNodeWidget(head);
    }

private:
    static QDomElement createNodeElement(QDomDocument& doc, NodeWidget* node){
        QDomElement e = doc.createElement("node");
        e.setAttribute("content", node->label().text());
        e.setAttribute("nodestyle", node->label().getNodeShape());
        e.setAttribute("font", node->label().font().toString());

        for(int i = 0 ; i< node->getChild().size(); i++){
            e.appendChild(createNodeElement(doc, node->getChild().at(i)));
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


        QString underlineCSS =  "border-top-style: none; border-right-style: none; border-bottom-style: solid; border-left-style: none; border-width: 2px;border-color: black;";
        QString recCSS = "border-width: 2px;border-style : solid;border-color: black;";
        QString roundRecCSS = "border-width: 2px; border-style : solid; border-radius: 4px; border-color: black;";
        QString nothingCSS = "border: 2px solid gray;";
        switch(e.attribute("nodestyle").toInt()){
        case 0:
            newNode->label().setStyleSheet(nothingCSS);
            break;
        case 1:
            newNode->label().setStyleSheet(recCSS);
            break;
        case 2:
            newNode->label().setStyleSheet(underlineCSS);
            break;
        case 3:
            newNode->label().setStyleSheet(roundRecCSS);
            break;

        }

        for(int i=0; i< e.childNodes().size();i++)
            newNode->add(createNodeWidget(e.childNodes().at(i).toElement()));

        return newNode;
    }
};

#endif // XMLHANDLER_H
