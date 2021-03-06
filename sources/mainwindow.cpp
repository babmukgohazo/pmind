#include "headers/mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFont>
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
    map = nullptr;
    process = new Process;
    container = new QWidget;
    containerLayout = new QHBoxLayout;

    scaleCombo = ui->scaleCombo;
    percentLabel = ui->percentLabel;

    scaleCombo->setEditable(true);
    scaleCombo->setInsertPolicy(QComboBox::NoInsert);
    scaleCombo->setFocusPolicy(Qt::ClickFocus);

    addDockWidget(Qt::RightDockWidgetArea,propertyDock);
    addDockWidget(Qt::RightDockWidgetArea,textDock);
    QMainWindow::tabifyDockWidget(propertyDock,textDock);
    propertyDock->raise();


    propertyDock->setMapScreen(mapScreen);

    QObject::connect(propertyDock,SIGNAL(fontChanged(NodeWidget*,QFont)),this,SLOT(addProcess(NodeWidget*,QFont)));
    QObject::connect(propertyDock,SIGNAL(nodeStyleChanged(NodeWidget*,nodeShape)),this,SLOT(addProcess(NodeWidget*,nodeShape)));

    mapScreen->setStyleSheet("MindmapView {border: 1px solid gray; background: #f8f8f8;}");
    setCentralWidget(mapScreen);

    mapScreen->mainWindow = this;

    QObject::connect(mapScreen, SIGNAL(zoomSignal()),this,SLOT(scaleCombo_setCurrentScale()));
    NodeWidget::setMainWindow(this);

    qGA = new QGoogleAnalytics(this);
    qGA->sendPageView("mindmapview");


    statusBar()->addWidget(scaleCombo);
    statusBar()->addWidget(percentLabel);

    containerLayout->setMargin(100);
    container->setLayout(containerLayout);
    mapScreen->mindmapScene->addWidget(container);
    container->setStyleSheet("background-color: rgba(255, 255, 255, 10);");

    createDockWindows();
}

MainWindow::~MainWindow()
{
    delete ui;
    if (map!=nullptr){
        delete map;
        map = nullptr;
    }
    delete mapScreen;
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
    map->labelPointer()->setNodeShape(nodeShape::root);
    map->labelPointer()->setDefaultColor(100);
    QFont *font = new QFont("Arial");
    font->setPointSize(18);
    map->labelPointer()->setFont(*font);

    containerLayout->addWidget(map);
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
    map->labelPointer()->setNodeShape(nodeShape::root);
    map->labelPointer()->setDefaultColor(100);
    QFont *font = new QFont("Arial");
    font->setPointSize(18);
    map->labelPointer()->setFont(*font);

    containerLayout->addWidget(map);
    renewTextEdit();
    propertyDock->setNodeWidget(map);
    changeWindowTitle();
    process = new Process;
}

void MainWindow::openFile(){
    if(contentChanged){
        QMessageBox msgBox(this);
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
        map->labelPointer()->setNodeShape(nodeShape::root);
        containerLayout->addWidget(map);
        propertyDock->setNodeWidget(map);
        process = new Process;
        renewTextEdit();
    }

    else if(m_fileName.endsWith(".md",Qt::CaseInsensitive)){

        //copy to text edit
        edit->setText("");
        QTextCodec *codec = QTextCodec::codecForName("EUC-KR");

        while (!file.atEnd()) {
            QByteArray line = file.readLine();
            edit->setText(edit->toPlainText().append(codec->toUnicode(line)));
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
    mx = container->rect().width();
    my = container->rect().height();

    NodeWidget* focusedNode = NodeWidget::searchFocusInNode(map);
    if(focusedNode!=nullptr){
        focusedNode->label().focusOut();
        focusedNode->textEditToLabel();
    }

    QImage image(QSize(mx*5,my*5),QImage::Format_RGB32);
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    mapScreen->getScene()->render(&painter);

    QString fileName = dialog.selectedFiles().first();
    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    QImage background(image.size(),QImage::Format_RGB32);
    background.fill(QColor("#f8f8f8"));
    QPainter backgroundPainter(&background);
    backgroundPainter.setCompositionMode(QPainter::CompositionMode_SourceAtop);
    backgroundPainter.drawImage(0,0,image);
    background.save(fileName);
}

void MainWindow::quit(){
    //just quit :)
    qGA->sendEvent("mindmapview", "quit", "quit");
    QApplication::instance()->quit();

}

void MainWindow::setFileMenuToolbar() {
    actionNew = new QAction("New");
    actionNew->setShortcut(QKeySequence(QKeySequence::New));
    actionOpen = new QAction("Open");
    actionOpen->setShortcut(QKeySequence(QKeySequence::Open));
    actionSave = new QAction("Save");
    actionSave->setShortcut(QKeySequence(QKeySequence::Save));
    actionSaveAs = new QAction("Save as");
    QList<QKeySequence> saveAsList;
    saveAsList << QKeySequence(tr("Ctrl+Shift+S")) << QKeySequence(QKeySequence::SaveAs);
    actionSaveAs->setShortcuts(saveAsList);
    actionExport = new QAction("Image export");
    actionExport->setShortcut(QKeySequence(tr("Ctrl+E")));
    actionQuit = new QAction("Quit");
    QList<QKeySequence> quitList;
    quitList << QKeySequence(tr("Ctrl+Q")) << QKeySequence(QKeySequence::Quit);
    actionQuit->setShortcuts(quitList);
    menuFile = new QMenu("Menu");
    viewMenu = new QMenu("View");
    menuFile->addAction(actionNew);
    menuFile->addAction(actionOpen);
    menuFile->addAction(actionSave);
    menuFile->addAction(actionSaveAs);
    menuFile->addAction(actionExport);
    menuFile->addSeparator();
    menuFile->addAction(actionQuit);
    this->menuBar()->addMenu(menuFile);
    this->menuBar()->addMenu(viewMenu);

    connect(actionNew, SIGNAL(triggered()), this, SLOT(newFile()));
    connect(actionOpen, SIGNAL(triggered()), this, SLOT(openFile()));
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
    case CommandType::Image:
        process->push(new ImageCommand(node));
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

void MainWindow::addProcess(NodeWidget *styleChangedNode, nodeShape shape){
    process->push(new NodeStyleCommand(styleChangedNode, shape));
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
    case Qt::Key_O:
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

void MainWindow::createDockWindows()
{
    viewMenu->addAction(propertyDock->toggleViewAction());
    viewMenu->addAction(textDock->toggleViewAction());
}
