#ifndef __FileOperations_h__
#define __FileOperations_h__

#include <QFile>
#include <QString>
#include <QDir>
#include <QtDebug>

class FileOperations {
public:
    bool copy(const QFile &file, QString destPath);
    bool move(QFile &file, QString destPath);
    bool remove(QFile &file);
    bool rename(QFile &file, const QString &newName);
    bool createSymlink(const QFile &target, const QDir &linkDir);
};

#endif // __FileOperations_h__
