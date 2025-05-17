#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <QFileSystemModel>
#include <QListView>
#include <QTreeView>
#include <QWidget>



class Directory : public QObject{

    Q_OBJECT

public:

    Directory(QTreeView* treeView, QListView* listView, QObject *parent);
    ~Directory();


private:
    QFileSystemModel *folderModel = nullptr;
    QFileSystemModel *fileModel = nullptr;

    QTreeView *treeView = nullptr;
    QListView *listView = nullptr;

    void setup();
};

#endif // DIRECTORY_H
