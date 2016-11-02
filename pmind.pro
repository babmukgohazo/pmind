#-------------------------------------------------
#
# Project created by QtCreator 2016-10-06T00:46:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pmind
TEMPLATE = app


SOURCES += \
    sources/main.cpp \
    sources/mainwindow.cpp \
    sources/parsing.cpp \
    sources/nodewidget.cpp

HEADERS  += \
    headers/mainwindow.h \
    headers/parsing.h \
    headers/mindmapview.h \
    headers/nodewidget.h

FORMS    += \
    forms/mainwindow.ui

RESOURCES += \
    resources/image.qrc
