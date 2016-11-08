#include "headers/mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFileMenuToolbar();
    setWindowTitle("P-mind");

    // construct & set UI component
    mapScreen = ui->graphicsView;
    edit = ui->textEdit;
    styleTabWidget=  ui->tabWidget;
    redrawButton = ui->reDraw;
    layout = new QHBoxLayout();
    rightLayout = new QVBoxLayout();
    map = nullptr;

    rightLayout->addWidget(styleTabWidget);
    rightLayout->addWidget(redrawButton);
    layout->addWidget(mapScreen);
    layout->addLayout(rightLayout);
    layout->setStretchFactor(mapScreen,7);
    layout->setStretchFactor(rightLayout,3);

    //mapScreen->setStyleSheet("MindmapView {border: 1px solid gray; background: white;}");
    this->centralWidget()->setLayout(layout);

    QObject::connect(redrawButton, SIGNAL(clicked()),this,SLOT(reload()));

    contentChanged = false;

    QObject::connect(edit, SIGNAL(textChanged()),this,SLOT(on_textEdit_textChanged()));

    //filemanage = new FileManage();
   // filemanage->setModal(true);
    //filemanage->show();

    //QObject::connect(filemanage, SIGNAL(signal_open()),this,SLOT(openFile()));
    QObject::connect(this, SIGNAL(signal_new()),this,SLOT(newFile()));

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
        delete map;
        map = nullptr;
    }
    QString str = edit->toPlainText();
    QQueue<MdString> q;
    getQqueue(str,q);
    map = new NodeWidget(q);
    mapScreen->mindmapScene->addWidget(map);
}


void MainWindow::newFile(){
    m_fileName = QDir::homePath() + "/untitled.pmind"; //use default file name
    edit->setText("");      //clear text editor
    contentChanged = false;
    if (map!=nullptr){      //delete mindmap
        delete map;
        map = nullptr;
    }
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
    contentChanged = false;

    //draw mindmap
    reload();
    changeWindowTitle();
}
void MainWindow::saveFile(){
    QFile file(m_fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        QMessageBox messageBox;
        messageBox.critical(0,"Error","파일을 저장하다가 오류가 났습니다.");
        return;
    }

    QTextStream out(&file);
    out << edit->toPlainText();
    file.close();
    contentChanged = false;
    changeWindowTitle();
}

void MainWindow::saveFileAs(){

    //simillar to openFile()
    QFileDialog dialog(this,
                       tr("Save Mindmap as"),
                       QDir::homePath(),
                       QString("P-mind (*.pmind)"));
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setDefaultSuffix("pmind");
    if (!dialog.exec())
        return;

    m_fileName = dialog.selectedFiles().first();
    saveFile();
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

void MainWindow::on_pushButton_11_clicked()
{
   // QPropertyAnimation *animation = new QPropertyAnimation(ui->label, "geometry");
    ui->widget->setVisible(false);
   // animation->setDuration(100);
    //animation->setStartValue(QRect(-400, 0, 100, 300));
    //animation->setEndValue(QRect(100, 0, 100, 300));
    //animation->start();
    emit signal_new();
}
