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
    propertyDock = new PropertyTab;
    textDock = new TextView;
    edit = textDock->getTextEdit();
    edit->setReadOnly(true);
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

    //scaleComboLayout->addWidget(scaleCombo);
    //scaleComboLayout->addWidget(percentLabel);
    //scaleComboLayout->addStretch();

    //rightLayout->addWidget(dockWidget);
    addDockWidget(Qt::RightDockWidgetArea,propertyDock);
    addDockWidget(Qt::RightDockWidgetArea,textDock);
    QMainWindow::tabifyDockWidget(propertyDock,textDock);
    propertyDock->raise();

    //rightLayout->addWidget(redrawButton);



    layout->addLayout(scaleComboLayout);
    //layout->addLayout(programLayout);
    layout->addWidget(mapScreen);
    layout->setMargin(0);
    layout->setSpacing(3);

    propertyDock->setMapScreen(mapScreen);

    QObject::connect(propertyDock,SIGNAL(fontChanged(NodeWidget*,QFont)),this,SLOT(addProcess(NodeWidget*,QFont)));

    mapScreen->setStyleSheet("MindmapView {border: 1px solid gray; background: white;}");
    this->centralWidget()->setLayout(layout);

    mapScreen->mainWindow = this;

    QObject::connect(redrawButton, SIGNAL(clicked()),this,SLOT(reload()));

    QObject::connect(mapScreen, SIGNAL(zoomSignal()),this,SLOT(scaleCombo_setCurrentScale()));

    NodeWidget::setMainWindow(this);

    qGA = new QGoogleAnalytics(this);
    qGA->sendPageView("mindmapview");


    statusBar()->addWidget(scaleCombo);
    statusBar()->addWidget(percentLabel);
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
        delete map;
        map = nullptr;
    }
    QString str = edit->toPlainText();
    getQqueue(str,mdQueue);
    map = new NodeWidget(mdQueue, this);
    map->labelPointer()->setStyleSheet("border-width: 3px; border-style : solid; border-color: #aed339;");
    map->labelPointer()->setNodeShape(root);
    mapScreen->mindmapScene->addWidget(map);
    renewTextEdit();
    QObject::connect(mapScreen,SIGNAL(viewClicked()),map,SLOT(update()));
    propertyDock->setNodeWidget(map);
    process = new Process;
}

void MainWindow::renewTextEdit(){
    if (map==nullptr){
         return;
     }
    QString inText;
    mdQueue.clear();
    dfs_stack.clear();
    dfs_stack.push(map);

    dfs(dfs_stack, mdQueue);
    convertQqToText(inText, mdQueue);
    edit->setPlainText(inText);
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
    map->labelPointer()->setStyleSheet("border-width: 3px; border-style : solid; border-color: #aed339;");
    map->labelPointer()->setNodeShape(root);
    mapScreen->mindmapScene->addWidget(map);
    renewTextEdit();
    propertyDock->setNodeWidget(map);
    changeWindowTitle();
    process = new Process;
}

void MainWindow::openFile(){
    if(contentChanged){//������ �ٲ���ٸ�
        QMessageBox msgBox(this); //�޽��� �ڽ��� ����� �����
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
                       QString("P-mind (*.pmind);; md File (*.md)"));
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
            map->close();
        }
        map = XmlHandler::Xml2Mindmap(doc);
        map->labelPointer()->setStyleSheet("border-width: 3px; border-style : solid; border-color: #aed339;");
        map->labelPointer()->setNodeShape(root);
        mapScreen->mindmapScene->addWidget(map);
        propertyDock->setNodeWidget(map);
        process = new Process;
        renewTextEdit();
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
        messageBox.critical(0,"Error","������ �����ϴٰ� ������ �����ϴ�.");
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
                       QString("P-mind (*.pmind);; md File (*.md)"));
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

    int mx,my;
    mx = map->rect().width();
    my = map->rect().height();

    QImage image(QSize(mx*5,my*5),QImage::Format_RGB32);
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    mapScreen->getScene()->render(&painter);

    QString fileName = dialog.selectedFiles().first();
    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    QImage background(image.size()+QSize(40,40),QImage::Format_RGB32);
    background.fill(Qt::white);
    QPainter backgroundPainter(&background);
    backgroundPainter.setCompositionMode(QPainter::CompositionMode_SourceAtop);
    backgroundPainter.drawImage(20,20,image);
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

void MainWindow::addProcess(NodeWidget *fontChangedNode, QFont lastFont){
    process->push(new FontCommand(fontChangedNode, lastFont));
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

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return)
    {
        bool ok;
        int scale = (scaleCombo->currentText()).toInt(&ok);
        if (ok == true)
            mapScreen->adjustScale(scale);
    }
    switch(e->key()){
    case Qt::Key_Z:
        if(e->modifiers().testFlag(Qt::ControlModifier))
            process->undo();
        break;
    case Qt::Key_Y:
        if(e->modifiers().testFlag(Qt::ControlModifier))
            process->redo();
        break;
    case Qt::Key_N:
        if(e->modifiers().testFlag(Qt::ControlModifier))
            newFile();
        break;
    case Qt::Key_S:
        if(e->modifiers().testFlag(Qt::ControlModifier))
            saveFile();
        else if(e->modifiers().testFlag(Qt::ShiftModifier))
            saveFileAs();
       break;
    case Qt::Key_L:
        if(e->modifiers().testFlag(Qt::ControlModifier))
            openFile();
        break;
    case Qt::Key_Q:
        if(e->modifiers().testFlag(Qt::ControlModifier))
            quit();
        break;
    case Qt::Key_E:
        if(e->modifiers().testFlag(Qt::ControlModifier))
            imageExport();
        break;
    default:
        break;
    }
}
