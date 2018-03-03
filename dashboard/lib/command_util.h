#ifndef COMMAND_UTIL_H
#define COMMAND_UTIL_H

#include "stacer-core_global.h"

#include <QStringList>


class STACERCORESHARED_EXPORT CommandUtil
{

public:
    static QString sudoExec(const QString &cmd, QStringList args = QStringList());
    static QString exec(const QString &cmd, QStringList args = QStringList());
    static bool isExecutable(const QString &cmd);

private:
    CommandUtil();

};

#endif // COMMAND_UTIL_H
