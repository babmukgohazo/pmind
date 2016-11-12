#include "headers/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFileMenuToolbar();
    setWindowTitle("P-mind");
    // construct & set UI component
    mapScreen = new MindmapView();
    edit = new QTextEdit();
    redrawButton = new QPushButton("Redraw");
    layout = new QHBoxLayout();
    rightLayout = new QVBoxLayout();
    map = nullptr;

    rightLayout->addWidget(edit);
    rightLayout->addWidget(redrawButton);
    layout->addWidget(mapScreen);
    layout->addLayout(rightLayout);
    layout->setStretchFactor(mapScreen,7);
    layout->setStretchFactor(rightLayout,3);

    mapScreen->setStyleSheet("MindmapView {border: 1px solid gray; background: white;}");
    this->centralWidget()->setLayout(layout);

    QObject::connect(redrawButton, SIGNAL(clicked()),this,SLOT(reload()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete redrawButton;
    delete rightLayout;
    delete edit;
    if (map!=nullptr){
        delete map;
        map = nullptr;
    }
    delete mapScreen;
    delete layout;
}

//re-allocate & re-draw mindmap
void MainWindow::reload(){
   if (map!=nullptr){
        QObject::disconnect(mapScreen,SIGNAL(viewClicked()),map,SLOT(update()));
        delete map;
        map = nullptr;
    }
    QString str = edit->toPlainText();
    QQueue<MdString> q;
    getQqueue(str,q);
    map = new NodeWidget(q, this);
    mapScreen->mindmapScene->addWidget(map);
    QObject::connect(mapScreen,SIGNAL(viewClicked()),map,SLOT(update()));
}

void MainWindow::renewTextEdit(){

}

void MainWindow::newFile(){
    m_fileName = "NewFile"; //use default file name
    edit->setText("");      //clear text editor
    if (map!=nullptr){      //delete mindmap
        delete map;
        map = nullptr;
    }
}

void MainWindow::openFile(){

    //file explorer. file extension is *.pmind (really same with *.txt)
    QFileDialog dialog(this,
                       tr("Open Mindmap"),
                       QDir::homePath(),
                       QString("P-mind (*.pmind)"));
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setDefaultSuffix("pmind");
    if (!dialog.exec())
        return;

    m_fileName = dialog.selectedFiles().first();    //get path & filename (ex: C:/Users/KIMJEONGHUN/test.pmind )

    //open file
    QFile file(m_fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    //copy to text edit
    edit->setText("");
    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        edit->setText(edit->toPlainText().append(line));
    }
    file.close();

    //draw mindmap
    reload();
}
void MainWindow::saveFile(){

    //wow very simple :)
    QFile file(m_fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    out << edit->toPlainText();
    file.close();
}

bool MainWindow::saveFileAs(){

    //simillar to openFile()
    QFileDialog dialog(this,
                       tr("Save Mindmap as"),
                       QDir::homePath(),
                       QString("P-mind (*.pmind)"));
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setDefaultSuffix("pmind");

    if (!dialog.exec())
        return false;

    m_fileName = dialog.selectedFiles().first();
    saveFile();
    return true;
}

void MainWindow::quit(){
    //just quit :)
    QApplication::instance()->quit();
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
    connect(actionSave, SIGNAL(triggered()), this, SLOT(saveFile()));
    connect(actionSaveAs, SIGNAL(triggered()), this, SLOT(saveFileAs()));
    connect(actionQuit, SIGNAL(triggered()), this, SLOT(quit()));
}
