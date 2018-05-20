#ifndef COMMAND_UTIL_H
#define COMMAND_UTIL_H

#include "stacer-core_global.h"

#include <QStringList>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QDirIterator>
#include <QStandardPaths>
#include <QSharedPointer>

class STACERCORESHARED_EXPORT CommandUtil
{

public:
    static QString sudoExec(const QString &cmd, QStringList args = QStringList());
    static QString exec(const QString &cmd, QStringList args = QStringList());
    static bool isExecutable(const QString &cmd);

    static QString readStringFromFile(const QString &path, const QIODevice::OpenMode &mode = QIODevice::ReadOnly);
    static QStringList readListFromFile(const QString &path, const QIODevice::OpenMode &mode = QIODevice::ReadOnly);
private:
    CommandUtil();

};

#endif // COMMAND_UTIL_H
