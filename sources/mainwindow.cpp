#include "headers/mainwindow.h"
#include "ui_mainwindow.h"
#include "headers/editscreen.h"
#include "headers/firstscreen.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFileMenuToolbar();


    showStartscreen();



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showEditscreen(){
    EditScreen *center = new EditScreen();
    this->setCentralWidget(center);
}

void MainWindow::showStartscreen(){
    FirstScreen *center = new FirstScreen();
    this->setCentralWidget(center);
}

void MainWindow::newFile(){
    showEditscreen();
}

void MainWindow::openFile(const QString &fileName){

    QFileDialog dialog(this,
                       tr("Open MindMap"),
                       QDir::homePath(),
                       QString("QtMindMap (*.qmm)"));
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setDefaultSuffix("qmm");

    if (!dialog.exec())
        return;

    m_fileName = dialog.selectedFiles().first();

}

void MainWindow::setFileMenuToolbar() {
    actionNew = new QAction("New");
    actionLoad = new QAction("Load");
    actionSave = new QAction("Save");
    actionSaveAs = new QAction("Save as");
    actionQuit = new QAction("Quit");
    menuFile = new QMenu("Menu");
    menuFile->addAction(actionNew);
    menuFile->addAction(actionLoad);
    menuFile->addAction(actionSave);
    menuFile->addAction(actionSaveAs);
    menuFile->addSeparator();
    menuFile->addAction(actionQuit);
    this->menuBar()->addMenu(menuFile);

    connect(actionNew, SIGNAL(triggered()), this, SLOT(newFile()));
    connect(actionLoad, SIGNAL(triggered()), this, SLOT(openFile()));
}
