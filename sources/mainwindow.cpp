#include "headers/mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "headers/xmlhandler.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFileMenuToolbar();
    setWindowTitle("P-mind");
    // construct & set UI component
    mapScreen = new MindmapView();
    dockWidget = new PropertyTab;
    edit = dockWidget->getTextEdit();
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

    //rightLayout->addWidget(dockWidget);
    addDockWidget(Qt::RightDockWidgetArea,dockWidget);
    //rightLayout->addWidget(redrawButton);

    programLayout->addWidget(mapScreen);
    programLayout->addLayout(rightLayout);
    programLayout->setStretchFactor(mapScreen,7);
    programLayout->setStretchFactor(rightLayout,3);

    layout->addLayout(scaleComboLayout);
    //layout->addLayout(programLayout);
    layout->addWidget(mapScreen);

    QObject::connect(mapScreen,SIGNAL(undid()),process,SLOT(undo()));
    QObject::connect(mapScreen,SIGNAL(redid()),process,SLOT(redo()));
    mapScreen->setStyleSheet("MindmapView {border: 1px solid gray; background: white;}");
    this->centralWidget()->setLayout(layout);

    mapScreen->mainWindow = this;

    QObject::connect(redrawButton, SIGNAL(clicked()),this,SLOT(reload()));

    QObject::connect(mapScreen, SIGNAL(zoomSignal()),this,SLOT(scaleCombo_setCurrentScale()));

    NodeWidget::setMainWindow(this);

    qGA = new QGoogleAnalytics(this);
    qGA->sendPageView("mindmapview");
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
    m_fileName = QDir::homePath() + "/untitled.pmind"; //use default file name
    edit->setText("");      //clear text editor
    contentChanged = false;
    if (map!=nullptr){      //delete mindmap
        delete map;
        map = nullptr;
    }
    qGA->sendEvent("menu", "click", "newFile");
    map = new NodeWidget;
    mapScreen->mindmapScene->addWidget(map);
    dockWidget->setNodeWidget(map);
    changeWindowTitle();

}

void MainWindow::openFile(){
    if(contentChanged){//내용이 바뀌었다면
        QMessageBox msgBox(this); //메시지 박스를 띄워서 물어보고
        msgBox.setWindowTitle(tr("pMindMap"));
        msgBox.setText(tr("file is changed"));
        msgBox.setInformativeText(tr("save?"));
        msgBox.setStandardButtons(QMessageBox::Save |
                                 QMessageBox::Discard |
                                  QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();

        switch (ret) {
        case QMessageBox::Save:
        {
           saveFile();
            break;
        }
        case QMessageBox::Discard:
            break;
        case QMessageBox::Cancel:
            return;
        default:
            break;
        }
    }

    //file explorer. file extension is *.pmind (really same with *.txt)
    QFileDialog dialog(this,
                       tr("Open Mindmap"),
                       QDir::homePath(),
                       QString("P-mind (*.pmind);; xml File (*.md)"));
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setDefaultSuffix("pmind");
    if (!dialog.exec())
        return;
    qGA->sendEvent("menu", "click", "openFile");
    m_fileName = dialog.selectedFiles().first();    //get path & filename (ex: C:/Users/KIMJEONGHUN/test.pmind )

    //open file
    QFile file(m_fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    if(m_fileName.endsWith(".pmind",Qt::CaseInsensitive)){
        //open file
        QDomDocument doc;
        if (!doc.setContent(&file))
        {
            file.close();
            return;
        }
        file.close();

        if(map != nullptr){
            delete map;
        }
        map = XmlHandler::Xml2Mindmap(doc);
        mapScreen->mindmapScene->addWidget(map);
        dockWidget->setNodeWidget(map);
    }

    else if(m_fileName.endsWith(".md",Qt::CaseInsensitive)){

        //copy to text edit
        edit->setText("");
        while (!file.atEnd()) {
            QByteArray line = file.readLine();
            edit->setText(edit->toPlainText().append(line));
        }
        file.close();
        reload();
    }
    changeWindowTitle();
}
void MainWindow::saveFile(){
    QFile file(m_fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        QMessageBox messageBox;
        messageBox.critical(0,"Error","파일을 저장하다가 오류가 났습니다.");
        return;
    }
    qGA->sendEvent("menu", "click", "saveFile");

    if(m_fileName.endsWith(".pmind",Qt::CaseInsensitive)){

        QTextStream out(&file);
        out << XmlHandler::mindmap2Xml(map);
    }

    else if(m_fileName.endsWith(".md",Qt::CaseInsensitive)){

        QTextStream out(&file);
        out << edit->toPlainText();
    }

    file.close();
    contentChanged = false;
    changeWindowTitle();
}

void MainWindow::saveFileAs(){

    //simillar to openFile()
    QFileDialog dialog(this,
                       tr("Save Mindmap as"),
                       QDir::homePath(),
                       QString("P-mind (*.pmind);; xml File (*.xml)"));
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setDefaultSuffix("pmind");
    if (!dialog.exec())
        return;
    qGA->sendEvent("menu", "click", "saveFileAs");
    m_fileName = dialog.selectedFiles().first();
    saveFile();
}

void MainWindow::imageExport(){
    if(map==nullptr)
        return;
    QFileDialog dialog(this,
                       tr("Image export"),
                       QDir::homePath(),
                       tr("PNG (*.png);;JPEG (*.jpeg);;BMP (*.bmp)"));
    dialog.setAcceptMode(QFileDialog::AcceptSave);

    if(!dialog.exec())
        return;
    qGA->sendEvent("menu", "click", "imageExport");

    QPixmap pixmap;
    QString fileName = dialog.selectedFiles().first();
    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    pixmap = map->grab();
    QImage background(pixmap.size()+QSize(40,40),QImage::Format_RGB32);
    background.fill(Qt::white);
    QPainter painter(&background);
    painter.setCompositionMode(QPainter::CompositionMode_SourceAtop);
    painter.drawImage(20,20,pixmap.toImage());
    background.save(fileName);
}

void MainWindow::quit(){
    //just quit :)
    qGA->sendEvent("mindmapview", "quit", "quit");
    QApplication::instance()->quit();

}

void MainWindow::setFileMenuToolbar() {
    actionNew = new QAction("New");
    actionLoad = new QAction("Load");
    actionSave = new QAction("Save");
    actionSaveAs = new QAction("Save as");
    actionExport = new QAction("Image export");
    actionQuit = new QAction("Quit");
    menuFile = new QMenu("Menu");
    menuFile->addAction(actionNew);
    menuFile->addAction(actionLoad);
    menuFile->addAction(actionSave);
    menuFile->addAction(actionSaveAs);
    menuFile->addAction(actionExport);
    menuFile->addSeparator();
    menuFile->addAction(actionQuit);
    this->menuBar()->addMenu(menuFile);

    connect(actionNew, SIGNAL(triggered()), this, SLOT(newFile()));
    connect(actionLoad, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(actionSave, SIGNAL(triggered()), this, SLOT(saveFile()));
    connect(actionSaveAs, SIGNAL(triggered()), this, SLOT(saveFileAs()));
    connect(actionExport,SIGNAL(triggered()),this,SLOT(imageExport()));
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
    qGA->sendEvent("scale", "adjust", "select", ratio);
}


void MainWindow::scaleCombo_setCurrentScale()
{
    QString curScale = QString::number(mapScreen->getCurrentScale());
    QStringList intScale = curScale.split('.');
    scaleCombo->setEditText(intScale[0]);
    qGA->sendEvent("scale", "adjust", "input", intScale[0].toInt());
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
