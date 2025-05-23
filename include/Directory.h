#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <QFileSystemModel>
#include <QListView>
#include <QTreeView>
#include <QWidget>
#include <QPushButton>
#include <QStack>
#include <FileOperations.h>
#include <QMenu>
#include <QAction>



class Directory : public QObject{

    Q_OBJECT

public:

    Directory(QTreeView* treeView, QListView* listView,
              QPushButton* backButton, QPushButton* forwardButton,
              QPushButton* ListGridButton, QObject *parent);
    ~Directory();


private:
    QFileSystemModel *folderModel = nullptr;
    QFileSystemModel *fileModel = nullptr;

    QTreeView *treeView = nullptr;
    QListView *listView = nullptr;

    QPushButton *backButton;
    QPushButton *forwardButton;
    QPushButton *ListGridButton;

    FileOperations fileOps;
    QString lastCopiedPath;

    void setup();

private slots:
    void showContextMenu(const QPoint &pos);

    QString currentPath();

};

#endif // DIRECTORY_H
