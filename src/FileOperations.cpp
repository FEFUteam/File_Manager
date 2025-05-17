#include "FileOperations.h"
#include <QFileInfo>
#include <QDir>

bool FileOperations::copy(const QFile &file, const QString &destPath) {
    if (!file.exists()) {
        qWarning() << "FileOperations::copy(): Source file does "
                "not exist -" << file.fileName();
        return false;
    }

    if (QFile::exists(destPath)) {
        qWarning() << "FileOperations::copy(): Destination file "
            "already exists -" << destPath;
        return false;
    }

    if (!QFile::copy(file.fileName(), destPath)) {
        qWarning() << "FileOperations::copy():" << file.errorString();
        return false;
    }

    return true;
}

bool FileOperations::move(QFile &file, const QString &destPath) {

    if (!file.exists()) {
        qWarning() << "FileOperations::move(): Source file does "
                "not exist -" << file.fileName();
        return false;
    }
    
    if (QFile::exists(destPath)) {
        qWarning() << "FileOperations::move(): Destination file "
                "already exists -" << destPath;
        return false;
    }

    if (file.rename(destPath))
        return true;

    qWarning() << "FileOperations::move():" << file.errorString();
    return false;
}

bool FileOperations::remove(QFile &file) {

    if (!file.exists()) {
        qWarning() << "FileOperations::remove(): File does not exist -" 
            << file.fileName();
        return false;
    }

    if (!file.remove()) {
        qWarning() << "FileOperations::remove():" << file.errorString();
        return false;
    }

    return true;
}

bool FileOperations::rename(QFile &file, const QString &newName) {

    if (!file.exists()) {
        qWarning() << "FileOperations::rename(): File does not exist -" 
                        << file.fileName();
        return false;
    }

    if (!file.rename(newName)) {
        qWarning() << "FileOperations::rename():" << file.errorString();
        return false;
    }

    return true;
}

bool FileOperations::createSymlink(const QFile &target, const QDir &linkDir) {

    if (!target.exists()) {
        qWarning() << "FileOperations::createSymlink(): "
            "Target file does not exist -" << target.fileName();
        return false;
    }

    QString linkPath = linkDir.filePath(target.fileName());
    
    if (QFile::exists(linkPath)) {
        qWarning() << "FileOperations::createSymlink(): "
            "Link path already exists -" << linkPath;
        return false;
    }
    
    bool succ = QFile::link(target.fileName(), linkPath);
    if (!succ) {
        qWarning() << "FileOperations::createSymlink(): "
            "symlink was not created because of "
            "unknown error -" << linkPath;
        return false;
    }

    return succ;
}
