#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qtextedit.h"
#include "qfiledialog.h"
#include "qtextstream.h"
#include "qmessagebox.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QMenuBar * menuBar = this->menuBar();
    QMenu * fileMenu = menuBar->addMenu(tr("&File"));
    QToolBar * toolBar = this->addToolBar(tr("File"));

    QAction * openAction = new QAction(QIcon(":/images/open.png"), tr("&New..."), this);
    QAction * saveAction = new QAction(QIcon(":/images/save.png"), tr("&Save..."), this);
    QAction * quitAction = new QAction(QIcon(":/images/quit.png"), tr("&Quit..."), this);

    openAction->setShortcut(tr("Ctrl+O"));
    openAction->setToolTip((tr("Open file")));
    saveAction->setShortcut(tr("Ctrl+S"));
    saveAction->setToolTip((tr("Save file")));
    quitAction->setShortcut(tr("Ctrl+Q"));
    quitAction->setToolTip((tr("Quit")));

    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(quitAction);

    toolBar->addAction(openAction);
    toolBar->addAction(saveAction);
    toolBar->addAction(quitAction);

    text = new QTextEdit(this);
    setCentralWidget(text);

    QObject::connect(openAction, SIGNAL(triggered()), this, SLOT(openFile()));
    QObject::connect(saveAction, SIGNAL(triggered()), this, SLOT(saveFile()));
    QObject::connect(quitAction, SIGNAL(triggered()), this, SLOT(quitFile()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openFile() {
    qDebug("Open");
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open file"), ":/open", tr("HTML files(*.html)"));

    QFile file(qPrintable(fileName));
    file.open(QIODevice::ReadOnly | QIODevice::Text);

    QTextStream stream(&file);
    QString str="";
    str = stream.readAll();

    text->setHtml(str);

}

void MainWindow::saveFile() {
    qDebug("Save");
    QString fileName = QFileDialog::getSaveFileName(this,tr("Save file"), ":/saved", tr("Text files(*.txt)"));

    QFile file(qPrintable(fileName));
    file.open(QIODevice::WriteOnly);

    QTextStream stream(&file);
    QString str = "";

    str=text->toPlainText();
    stream << str;

}

void MainWindow::quitFile() {
    qDebug("Quit");
    QMessageBox * msgBox = new QMessageBox(this);

    msgBox->setText(tr("Are you sur you want to quit ?"));
    msgBox->setStandardButtons(QMessageBox::No|QMessageBox::Yes);
    int ret = msgBox->exec();
    if (ret == QMessageBox::Yes)
        this->close();

}

void MainWindow::closeEvent(QCloseEvent *e) {
    this->quitFile();
}
