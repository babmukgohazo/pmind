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
    dockWidget = new PropertyTab(this);
    edit = new QTextEdit();
    redrawButton = new QPushButton("Redraw");
    layout = new QVBoxLayout();
    programLayout = new QHBoxLayout();
    rightLayout = new QVBoxLayout();
    map = nullptr;
    process = new Process;

    scaleComboLayout = new QHBoxLayout;
    scaleCombo = ui->scaleCombo;
    percentLabel = ui->percentLabel;

    scaleCombo->setEditable(true);
    scaleCombo->setInsertPolicy(QComboBox::NoInsert);
    scaleCombo->setFocusPolicy(Qt::ClickFocus);

    scaleComboLayout->addWidget(scaleCombo);
    scaleComboLayout->addWidget(percentLabel);
    scaleComboLayout->addStretch();

    rightLayout->addWidget(dockWidget);
    rightLayout->addWidget(redrawButton);

    programLayout->addWidget(mapScreen);
    programLayout->addLayout(rightLayout);
    programLayout->setStretchFactor(mapScreen,7);
    programLayout->setStretchFactor(rightLayout,3);

    layout->addLayout(scaleComboLayout);
    layout->addLayout(programLayout);

    QObject::connect(mapScreen,SIGNAL(undid()),process,SLOT(undo()));
    QObject::connect(mapScreen,SIGNAL(redid()),process,SLOT(redo()));
    mapScreen->setStyleSheet("MindmapView {border: 1px solid gray; background: white;}");
    this->centralWidget()->setLayout(layout);

    mapScreen->mainWindow = this;

    QObject::connect(redrawButton, SIGNAL(clicked()),this,SLOT(reload()));
    QObject::connect(mapScreen, SIGNAL(zoomSignal()),this,SLOT(scaleCombo_setCurrentScale()));


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
    //delete process;
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
    dockWidget->setNodeWidget(map);
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

void MainWindow::addProcess(NodeWidget* node, CommandType type){
    switch(type){
    case CommandType::Add:
        process->push(new AddCommand(node));
        break;
    case CommandType::Delete:
        process->push(new DeleteCommand(node));
        break;
    case CommandType::Text:
        process->push(new TextCommand(node));
        break;
    }
}

void MainWindow::addProcess(NodeWidget *movedNode, NodeWidget *to, CommandType type){
    switch(type){
    case CommandType::Move:
        process->push(new MoveCommand(movedNode, to));
        break;
    }
}

void MainWindow::on_scaleCombo_currentIndexChanged(const QString &arg1)
{
    bool ok;
    int ratio = arg1.toInt(&ok,10);
    mapScreen->adjustScale(ratio);
}


void MainWindow::scaleCombo_setCurrentScale()
{
    QString curScale = QString::number(mapScreen->getCurrentScale());
    QStringList intScale = curScale.split('.');
    scaleCombo->setEditText(intScale[0]);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
    {
        bool ok;
        int scale = (scaleCombo->currentText()).toInt(&ok);
        if (ok == true)
            mapScreen->adjustScale(scale);
    }
}
