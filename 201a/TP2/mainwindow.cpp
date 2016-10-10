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
    QMenu * editMenu = menuBar->addMenu(tr("Edit"));
    QMenu * deleteMenu = menuBar->addMenu(tr("Delete"));

    canvas = new Canvas(this);
    setCentralWidget(canvas);


    /* ACTIONS FOR OPEN SAVE & QUIT */
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

    QObject::connect(openAction, SIGNAL(triggered()), this, SLOT(openFile()));
    QObject::connect(saveAction, SIGNAL(triggered()), this, SLOT(saveFile()));
    QObject::connect(quitAction, SIGNAL(triggered()), this, SLOT(quitFile()));


    /* ACTIONS FOR BRUSH OPTIONS */

    /* GROUP & CONNECT FOR WIDTH */
    QActionGroup * widthGroup = new QActionGroup(this);
    QAction * wideAction = new QAction(tr("Wide"), widthGroup);
    QAction * thinAction = new QAction(tr("Thin"), widthGroup);

    QObject::connect(widthGroup, SIGNAL(triggered(QAction*)),
                canvas, SLOT(setWidth(QAction*)));

    /* GROUP & CONNECT FOR COLOR */
    QActionGroup * colorGroup = new QActionGroup(this);
    QAction * blackAction = new QAction(tr("Black"), colorGroup);
    QAction * blueAction = new QAction(tr("Blue"), colorGroup);

    QObject::connect(colorGroup, SIGNAL(triggered(QAction*)),
                canvas, SLOT(setColor(QAction*)));


    /* GROUP & CONNECT FOR STYLE */
    QActionGroup * styleGroup = new QActionGroup(this);
    QAction * solidAction = new QAction(tr("Solid"), styleGroup);
    QAction * dashAction = new QAction(tr("Dash"), styleGroup);

    QObject::connect(styleGroup, SIGNAL(triggered(QAction*)),
                canvas, SLOT(setStyle(QAction*)));

    editMenu->addAction(thinAction);
    editMenu->addAction(wideAction);
    editMenu->addAction(blackAction);
    editMenu->addAction(blueAction);
    editMenu->addAction(solidAction);
    editMenu->addAction(dashAction);

    /* GROUP AND CONNECT FOR REMOVING */
    QActionGroup * deleteGroup = new QActionGroup(this);
    QAction * deleteLastAction = new QAction(tr("Delete last"), deleteGroup);
    QAction * deleteAllAction = new QAction(tr("Delete all"), deleteGroup);

    QObject::connect(deleteGroup, SIGNAL(triggered(QAction*)),
                     canvas, SLOT(deleteLine(QAction*)));

    deleteMenu->addAction(deleteAllAction);
    deleteMenu->addAction(deleteLastAction);

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

    if (fileName != NULL)
    {
        file.open(QIODevice::WriteOnly);

        QTextStream stream(&file);
        QString str = "";

        str=text->toPlainText();
        stream << str;
    }

}

void MainWindow::quitFile() {
    qDebug("Quit");
//    QApplication::sendEvent(this, new QCloseEvent());
    close();
}

void MainWindow::closeEvent(QCloseEvent *e) {
    QMessageBox * msgBox = new QMessageBox(this);

    msgBox->setText(tr("Are you sur you want to quit ?"));
    msgBox->setStandardButtons(QMessageBox::Yes|QMessageBox::No);
    int ret = msgBox->exec();
    if (ret == QMessageBox::Yes)
        e->accept();
    else
        e->ignore();
}
