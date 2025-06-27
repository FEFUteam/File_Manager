#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Directory.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    new Directory(ui->treeView, ui->listView, ui->BackPush, ui->ForwardPush, ui->ListGridButton, this);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_ListGridButton_toggled(bool checked)
{
    if (checked) {



        ui->ListGridButton->setIcon(QIcon(":/icons/icons/grid_active.png"));
        ui->ListGridButton->setIconSize(QSize(24, 24));
    } else {



        ui->ListGridButton->setIcon(QIcon(":/icons/icons/icons8-grid-48.png"));
        ui->ListGridButton->setIconSize(QSize(24, 24));
    }
}

