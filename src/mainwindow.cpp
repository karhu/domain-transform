#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>

int createWindow(int argc, char *argv[]) {
    QApplication a(argc,argv);
    MainWindow w;
    w.setWindowTitle( "Domain Transform" );

    w.show();
    return a.exec();
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "",
                                                    tr("PNG (*.png)"));
//                                                  tr("Files (*.*)"));
}
