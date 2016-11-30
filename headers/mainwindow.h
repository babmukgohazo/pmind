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
#include <QComboBox>
#include <QDebug>
#include "headers/mindmapview.h"
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
    Process* getProcess(){return process;}

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

    void addProcess(NodeWidget*, CommandType);
    void addProcess(NodeWidget*, NodeWidget*, CommandType);

private slots:
    void on_scaleCombo_currentIndexChanged(const QString& arg1);
    void scaleCombo_setCurrentScale();

protected:
    void keyPressEvent(QKeyEvent *event);

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
    QVBoxLayout* rightLayout;

    // FileMenu Toolbar actions
    QMenu *menuFile;
    QAction *actionNew;
    QAction *actionLoad;
    QAction *actionSave;
    QAction *actionSaveAs;
    QAction *actionQuit;

    Process* process;

    QVBoxLayout* layout;
    QHBoxLayout* programLayout;
    QHBoxLayout* scaleComboLayout;
    QComboBox* scaleCombo;
    QLabel* percentLabel;

};

#endif // MAINWINDOW_H
