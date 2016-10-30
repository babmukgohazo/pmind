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
    sources/editscreen.cpp \
    sources/firstscreen.cpp

HEADERS  += \
    headers/arrangement.h \
    headers/mainwindow.h \
    headers/node.h \
    headers/tree.h \
    headers/editscreen.h \
    headers/firstscreen.h

FORMS    += \
    forms/mainwindow.ui

RESOURCES += \
    resources/image.qrc
