#include "headers/mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFileMenuToolbar();


    mapScreen = new MindmapView();
    edit = new QTextEdit();
    redrawButton = new QPushButton("Redraw");
    layout = new QHBoxLayout();
    rightLayout = new QVBoxLayout();
    map = new NodeWidget();

    rightLayout->addWidget(edit);
    rightLayout->addWidget(redrawButton);
    layout->addWidget(mapScreen);
    layout->addLayout(rightLayout);
    layout->setStretchFactor(mapScreen,7);
    layout->setStretchFactor(rightLayout,3);

    mapScreen->setObjectName("mapscreen");
    mapScreen->setStyleSheet("#mapscreen {border: 1px solid gray; background: white;}");
    this->centralWidget()->setLayout(layout);

    QObject::connect(redrawButton, SIGNAL(clicked()),this,SLOT(reload()));

}

MainWindow::~MainWindow()
{
    delete ui;
    delete redrawButton;
    delete rightLayout;
    delete edit;
    delete layout;
    delete mapScreen;
}

void MainWindow::newFile(){/*
    m_fileName = "newfile.qmm";
    mapFile = new QFile(m_fileName);
    mapFile->open(QIODevice::ReadWrite);
    //mapFile->read(5);
    mapFile->write("msg", qstrlen("msg"));        // write to stderr
    mapFile->close();*/
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

//    QFile file(m_fileName);
//    //file.open();

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
