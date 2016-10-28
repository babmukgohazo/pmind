#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void showStartscreen();
    void showEditscreen();

public slots:
    // filemenu actions
    void newFile();
    void openFile(const QString &fileName = "");
  //  void saveFile(const bool &checkIfReadonly = true);
  //  bool saveFileAs();
  //  bool closeFile();

   //  handle changed content at quit
   // void quit();

private:
    void setFileMenuToolbar();

    Ui::MainWindow *ui;
    QMenu *menuFile;
    QString m_fileName;

    // FileMenu Toolbar actions
    QAction *actionNew;
    QAction *actionLoad;
    QAction *actionSave;
    QAction *actionSaveAs;
    QAction *actionQuit;
};

#endif // MAINWINDOW_H
