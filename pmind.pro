#-------------------------------------------------
#
# Project created by QtCreator 2016-10-06T00:46:52
#
#-------------------------------------------------

QT       += core gui xml network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pmind
TEMPLATE = app


SOURCES += \
    sources/main.cpp \
    sources/mainwindow.cpp \
    sources/parsing.cpp \
    sources/nodewidget.cpp \
    forms/propertytab.cpp \
    sources/mindmapview.cpp \
    sources/process.cpp \
    forms/textview.cpp \
    sources/qgoogleanalytics.cpp

HEADERS  += \
    headers/mainwindow.h \
    headers/parsing.h \
    headers/mindmapview.h \
    headers/nodewidget.h \
    forms/propertytab.h \
    headers/process.h \
    headers/xmlhandler.h \
    forms/textview.h \
    headers/qgoogleanalytics.h

FORMS    += \
    forms/mainwindow.ui \
    forms/propertytab.ui \
    forms/textview.ui

RESOURCES += \
    resources/image.qrc
