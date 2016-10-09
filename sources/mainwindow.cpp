#include "headers/mainwindow.h"
#include "ui_mainwindow.h"
#include "headers/editscreen.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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
    QLabel *center = new QLabel("Start");
    this->setCentralWidget(center);
}

void MainWindow::newFile(){
    showEditscreen();
}
