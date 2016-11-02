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
#include "forms/filemanage.h"

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
    void saveFileAs();

    //handle changed content at quit
    void quit();

private slots:
    void on_textEdit_textChanged(){
        contentChanged = true;
        changeWindowTitle();
    }

private:
    void setFileMenuToolbar();
    void changeWindowTitle(){
        QString title;
        title = "P-mind     " + m_fileName;
        if(contentChanged)
            title += '*';
        setWindowTitle(title);
    }

    //system & data component
    Ui::MainWindow *ui;
    QString m_fileName;
    QString m_windowTitle;
    QString contents;
    bool contentChanged;  //textEdit 창이 바뀌었는지 확인
    FileManage* filemanage;

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
