#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QBoxLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QGraphicsView>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QKeyEvent>
#include <QDebug>
#include "headers/mindmapview.h"
#include "headers/nodewidget.h"
#include "headers/parsing.h"
#include "forms/propertytab.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void showStartscreen();
    MindmapView* getMapScreen(){return mapScreen;}
    NodeWidget* getMap(){return map;}

public slots:
    //slots for graphic & drawing
    void reload();
    void renewTextEdit();

    // filemenu actions
    void newFile();
    void openFile();
    void saveFile();
    bool saveFileAs();

    //handle changed content at quit
    void quit();

private:
    void setFileMenuToolbar();

    //system & data component
    Ui::MainWindow *ui;
    QString m_fileName;

    //widget & graphic component
    MindmapView* mapScreen;
    NodeWidget* map;
    PropertyTab * dockWidget;
    QTextEdit* edit;
    QPushButton* redrawButton;
    QHBoxLayout* layout;
    QVBoxLayout* rightLayout;

    // FileMenu Toolbar actions
    QMenu *menuFile;
    QAction *actionNew;
    QAction *actionLoad;
    QAction *actionSave;
    QAction *actionSaveAs;
    QAction *actionQuit;
};

#endif // MAINWINDOW_H
