#include "Directory.h"
#include <QDir>
#include <QFile>
#include <qboxlayout.h>



Directory::Directory(QTreeView* treeView, QListView* listView,
                     QPushButton* backButton, QPushButton* forwardButton,
                     QPushButton* ListGridButton, QObject *parent)
    : QObject(parent),
    folderModel(new QFileSystemModel(this)),
    fileModel(new QFileSystemModel(this)),
    treeView(treeView),
    listView(listView),
    backButton(backButton),
    forwardButton(forwardButton),
    ListGridButton(ListGridButton)
{
    setup();
}

Directory::~Directory(){}

void Directory::setup(){

    folderModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Hidden);
    folderModel->setRootPath("");
    treeView->setModel(folderModel);
    treeView->setRootIndex(folderModel->index(""));
    treeView->setHeaderHidden(true);
    for (int i = 1; i < folderModel->columnCount(); ++i)
        treeView->hideColumn(i);

    fileModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files | QDir::Hidden);
    fileModel->setRootPath("");
    listView->setModel(fileModel);
    listView->setRootIndex(fileModel->index(""));
    listView->setViewMode(QListView::IconMode);
    listView->setGridSize(QSize(100, 100));
    listView->setIconSize(QSize(64, 64));
    listView->setResizeMode(QListView::Adjust);
    listView->setMovement(QListView::Static);

    connect(treeView->selectionModel(), &QItemSelectionModel::currentChanged,
            this, [=](const QModelIndex &current, const QModelIndex &){
        QString path = folderModel->filePath(current);
        listView->setRootIndex(fileModel->setRootPath(path));
    });

    connect(listView, &QListView::doubleClicked, this, [=](const QModelIndex &index) {
        if (!index.isValid())
            return;

        QString path = fileModel->filePath(index);
        QFileInfo info(path);
        if (info.isDir()) {
            QModelIndex treeIndex = folderModel->index(path);
            if (treeIndex.isValid()) {
                treeView->setCurrentIndex(treeIndex);
                treeView->scrollTo(treeIndex);
            }
        }
    });

    connect(ListGridButton, &QPushButton::clicked, this, [=](){
        if (listView->viewMode() == QListView::ViewMode::IconMode){
            listView->setViewMode(QListView::ListMode);
            listView->setGridSize(QSize(32, 32));
            listView->setIconSize(QSize(32, 32));
        } else{
            listView->setViewMode(QListView::IconMode);
            listView->setGridSize(QSize(100, 100));
            listView->setIconSize(QSize(64, 64));
        }
    });

}





























