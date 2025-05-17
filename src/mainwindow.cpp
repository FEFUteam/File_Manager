#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Directory.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    new Directory(ui->DirTreeView, ui->DirListView, this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
