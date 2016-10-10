#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qtextedit.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void saveFile();
    void quitFile();
    void openFile();

private:
    Ui::MainWindow *ui;
    QTextEdit * text;
    void closeEvent(QCloseEvent * e);
};

#endif // MAINWINDOW_H
