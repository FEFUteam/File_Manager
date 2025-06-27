// Directory.cpp
#include "Directory.h"
#include <QDir>
#include <QFile>
#include <QDesktopServices>
#include <QUrl>
#include <QClipboard>
#include <QApplication>
#include <QMessageBox>
#include <QMenu>
#include <QFileInfo>
#include <QDateTime>
#include <QLineEdit>
#include <QInputDialog>


bool copyDirRecursive(const QString &sourcePath, const QString &destinationPath);

QString clipboardPath;
bool isCutOperation = false;

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

Directory::~Directory() {}

void Directory::showFileProperties(const QFileInfo &info) {
    QString details = QString(
                          "Имя файла: %1\n"
                          "Тип: %2\n"
                          "Расположение: %3\n"
                          "Размер: %4 байт\n"
                          "Содержит файлов: %5\n"
                          "Содержит папок: %6\n"
                          "Создан: %7")
                          .arg(info.fileName())
                          .arg(info.isDir() ? "Папка" : "Файл")
                          .arg(info.absoluteFilePath())
                          .arg(info.isFile() ? QString::number(info.size()) : "-")
                          .arg(info.isDir() ? QString::number(QDir(info.absoluteFilePath()).entryList(QDir::Files).count()) : "-")
                          .arg(info.isDir() ? QString::number(QDir(info.absoluteFilePath()).entryList(QDir::Dirs | QDir::NoDotAndDotDot).count()) : "-")
                          .arg(info.birthTime().toString("dd.MM.yyyy hh:mm:ss"));

    QMessageBox::information(nullptr, "Свойства", details);
}

void Directory::setupContextMenu() {
    listView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(listView, &QListView::customContextMenuRequested, this, [=](const QPoint &pos) {
        QModelIndex index = listView->indexAt(pos);
        QFileInfo info;
        bool clickedOnItem = index.isValid();

        QMenu contextMenu;
        QAction *openAction = nullptr;
        QAction *pasteAction = nullptr;
        QAction *renameAction = nullptr;

        if (clickedOnItem)
            info = QFileInfo(fileModel->filePath(index));

        if (clickedOnItem && !info.isDir()) {
            openAction = contextMenu.addAction("Открыть");
        }

        if (clickedOnItem) {
            contextMenu.addAction("Копировать", [=]() {
                clipboardPath = info.absoluteFilePath();
                isCutOperation = false;
            });

            contextMenu.addAction("Вырезать", [=]() {
                clipboardPath = info.absoluteFilePath();
                isCutOperation = true;
            });

            contextMenu.addAction("Удалить", [=]() {
                if (QMessageBox::question(nullptr, "Удалить", "Вы уверены, что хотите удалить файл?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
                    if (info.isDir()) {
                        QDir dir(info.absoluteFilePath());
                        dir.removeRecursively();
                    } else {
                        QFile::remove(info.absoluteFilePath());
                    }
                }
            });

            renameAction = contextMenu.addAction("Переименовать");
        }

        if (!clipboardPath.isEmpty()) {
            pasteAction = contextMenu.addAction("Вставить");
        }

        if (clickedOnItem) {
            contextMenu.addAction("Свойства", [=]() {
                showFileProperties(info);
            });
            contextMenu.addAction("Дублировать", [=]() {
                QFileInfo originalInfo = info;
                QString baseName = originalInfo.completeBaseName();
                QString extension = originalInfo.suffix();
                QString dirPath = originalInfo.dir().absolutePath();

                QString duplicateName;
                QString newPath;
                int counter = 1;

                do {
                    duplicateName = baseName + "_" + QString::number(counter);
                    if (!extension.isEmpty() && originalInfo.isFile())
                        duplicateName += "." + extension;
                    newPath = dirPath + QDir::separator() + duplicateName;
                    ++counter;
                } while (QFileInfo::exists(newPath));

                bool success = false;

                if (originalInfo.isDir()) {
                    success = copyDirRecursive(originalInfo.absoluteFilePath(), newPath);
                } else {
                    success = QFile::copy(originalInfo.absoluteFilePath(), newPath);
                }

                if (!success) {
                    QMessageBox::warning(nullptr, "Ошибка", "Не удалось создать копию.");
                }
            });
        }

        QAction *selectedAction = contextMenu.exec(listView->viewport()->mapToGlobal(pos));

        if (selectedAction == openAction) {
            QDesktopServices::openUrl(QUrl::fromLocalFile(info.absoluteFilePath()));
        } else if (selectedAction == pasteAction) {
            // Точка вставки: либо внутрь папки, либо в текущую открытую директорию
            QString targetDirPath;
            if (clickedOnItem && info.isDir()) {
                targetDirPath = info.absoluteFilePath();
            } else {
                targetDirPath = fileModel->filePath(listView->rootIndex());
            }

            QFileInfo srcInfo(clipboardPath);
            QString newPath = QDir(targetDirPath).filePath(srcInfo.fileName());

            if (srcInfo.isDir()) {
                QDir sourceDir(clipboardPath);
                QDir destDir(newPath);
                if (!destDir.exists()) {
                    QDir().mkpath(newPath);
                }
                foreach (QString file, sourceDir.entryList(QDir::Files)) {
                    QFile::copy(sourceDir.absoluteFilePath(file), newPath + QDir::separator() + file);
                }
            } else {
                QFile::copy(clipboardPath, newPath);
            }

            if (isCutOperation) {
                if (srcInfo.isDir()) {
                    QDir dir(clipboardPath);
                    dir.removeRecursively();
                } else {
                    QFile::remove(clipboardPath);
                }
                isCutOperation = false;
            }

            clipboardPath.clear();
        } else if (selectedAction == renameAction) {
            QString oldPath = info.absoluteFilePath();
            QString oldName = info.fileName();

            bool ok;
            QString newName = QInputDialog::getText(nullptr, "Переименование",
                                                    "Новое имя:", QLineEdit::Normal,
                                                    oldName, &ok);

            if (ok && !newName.isEmpty() && newName != oldName) {
                QString newPath = info.dir().absoluteFilePath(newName);
                bool success = false;

                if (info.isDir()) {
                    QDir dir;
                    success = dir.rename(oldPath, newPath);
                } else {
                    QFile file(oldPath);
                    success = file.rename(newPath);
                }

                if (!success) {
                    QMessageBox::warning(nullptr, "Ошибка", "Не удалось переименовать файл.");
                }
            }
        }
    });
}



void Directory::setup() {
    folderModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Hidden);
    folderModel->setRootPath("");
    treeView->setModel(folderModel);
    treeView->setRootIndex(folderModel->index(""));
    treeView->setHeaderHidden(true);
    for (int i = 1; i < folderModel->columnCount(); ++i)
        treeView->hideColumn(i);
    fileModel->setFilter(QDir::NoDot | QDir::AllDirs | QDir::Files | QDir::Hidden);
    fileModel->setRootPath("");
    listView->setModel(fileModel);
    listView->setRootIndex(fileModel->index(""));
    listView->setViewMode(QListView::IconMode);
    listView->setGridSize(QSize(100, 100));
    listView->setIconSize(QSize(64, 64));
    listView->setResizeMode(QListView::Adjust);
    listView->setMovement(QListView::Static);
    listView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(treeView->selectionModel(), &QItemSelectionModel::currentChanged,
            this, [=](const QModelIndex &current, const QModelIndex &) {
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

    connect(ListGridButton, &QPushButton::clicked, this, [=]() {
        if (listView->viewMode() == QListView::ViewMode::IconMode) {
            listView->setViewMode(QListView::ListMode);
            listView->setGridSize(QSize(32, 32));
            listView->setIconSize(QSize(32, 32));
        } else {
            listView->setViewMode(QListView::IconMode);
            listView->setGridSize(QSize(100, 100));
            listView->setIconSize(QSize(64, 64));
        }
    });
    setupContextMenu();
}



bool copyDirRecursive(const QString &sourcePath, const QString &destinationPath) {
    QDir sourceDir(sourcePath);
    if (!sourceDir.exists())
        return false;

    QDir destDir(destinationPath);
    if (!destDir.exists()) {
        if (!QDir().mkpath(destinationPath))
            return false;
    }

    QFileInfoList entries = sourceDir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries);
    for (const QFileInfo &entry : entries) {
        QString srcPath = entry.absoluteFilePath();
        QString dstPath = destinationPath + QDir::separator() + entry.fileName();

        if (entry.isDir()) {
            if (!copyDirRecursive(srcPath, dstPath))
                return false;
        } else {
            if (!QFile::copy(srcPath, dstPath))
                return false;
        }
    }

    return true;
}