#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QBoxLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QGraphicsView>
#include <QFile>
#include <QMenu>
#include <QFileDialog>
#include <QTextStream>
#include <QKeyEvent>
#include <QComboBox>
#include <QDebug>
#include <QSettings>
#include "headers/mindmapview.h"
#include "headers/qgoogleanalytics.h"
//#include "forms/propertytab.h"

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
    QWidget* getContainer(){return container;}
    PropertyTab* getPropertyDock(){return propertyDock;}
    TextView* getTextViewDock(){return textDock;}

public slots:
    //slots for graphic & drawing
    void reload();
    void renewTextEdit();

    // filemenu actions
    void newFile();
    void openFile();
    void saveFile();
    void saveFileAs();
    void imageExport();

    //handle changed content at quit
    void quit();

    void addProcess(NodeWidget*, CommandType);
    void addProcess(NodeWidget*, NodeWidget*, CommandType);
    void addProcess(NodeWidget*, QFont);
    void addProcess(NodeWidget*, nodeShape);

private slots:
    void on_scaleCombo_currentIndexChanged(const QString& arg1);
    void scaleCombo_setCurrentScale();

protected:
    void keyPressEvent(QKeyEvent *e);

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
    QQueue<MdString> mdQueue;
    QStack<NodeWidget*> dfs_stack;

    //widget & graphic component
    MindmapView* mapScreen;
    NodeWidget* map;
    PropertyTab* propertyDock;
    TextView* textDock;
    QTextEdit* edit;
    QPushButton* redrawButton;
    QVBoxLayout* rightLayout;
    QWidget* container;
    QHBoxLayout* containerLayout;

    // FileMenu Toolbar actions
    QMenu *menuFile;
    QAction *actionNew;
    QAction *actionOpen;
    QAction *actionSave;
    QAction *actionSaveAs;
    QAction *actionExport;
    QAction *actionQuit;

    Process* process;

    QVBoxLayout* layout;
    QHBoxLayout* programLayout;
    QHBoxLayout* scaleComboLayout;
    QComboBox* scaleCombo;
    QLabel* percentLabel;

    QGoogleAnalytics* qGA;
};

#endif // MAINWINDOW_H
