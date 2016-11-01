#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QBoxLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QGraphicsView>
#include <QFile>
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
    void showEditscreen();

public slots:
    void reload(){
        if (map!=nullptr)
            delete map;
        QString str = edit->toPlainText();
        QQueue<MdString> q;
        getQqueue(str,q);
        map = new NodeWidget(q);
        mapScreen->mindmapScene->addWidget(map);
    }
    // filemenu actions
    void newFile();
    void openFile(const QString &fileName = "");
//    void saveFile(const bool &checkIfReadonly = true);
//    bool saveFileAs();
//    bool closeFile();

//    //handle changed content at quit
//    void quit();

private:
    void setFileMenuToolbar();

    Ui::MainWindow *ui;
    QString m_fileName;
    QFile* mapFile;

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
