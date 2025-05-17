#include "Directory.h"
#include <QDir>
#include <QFile>



Directory::Directory(QTreeView* treeView, QListView* listView, QObject *parent)
    : QObject(parent),
    folderModel(new QFileSystemModel(this)),
    fileModel(new QFileSystemModel(this)),
    treeView(treeView),
    listView(listView)
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

    fileModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files | QDir::Hidden);
    fileModel->setRootPath("");
    listView->setModel(fileModel);
    listView->setRootIndex(fileModel->index(""));

    connect(treeView->selectionModel(), &QItemSelectionModel::currentChanged, this, [=](const QModelIndex &current, const QModelIndex &){
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

}

