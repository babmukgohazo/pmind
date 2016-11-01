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
#include "headers/mindmapview.h"
#include "headers/nodewidget.h"
#include "headers/parsing.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void showStartscreen();

public slots:
    //slots for graphic & drawing
    void reload();

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
