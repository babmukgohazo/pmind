#include "headers/mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();


    return a.exec();
}

/*
# HEAD
## 1
## 2
### 2 - 1
### 2 - 2
- 2 - 2 - 1
- 2 - 2 - 2
- 2 - 2 - 3
    - 2- 2- 3 - 1
    - 2- 2- 3 - 2
## 3
### 3 - 1
### 3 - 2
### 3 - 3
### 3 - 4
## 4
### 4 - 1

*/
