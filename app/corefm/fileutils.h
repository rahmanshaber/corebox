/*
CoreBox is combination of some common desktop apps.

CoreBox is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; version 2
of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, see {http://www.gnu.org/licenses/}. */

#ifndef FILEUTILS_H
#define FILEUTILS_H

#include "progressdlg.h"
#include "desktopfile.h"
#include "properties.h"

#include <QObject>
#include <QFileInfo>
#include <QDirIterator>
#include <sys/vfs.h>


class ProgressWatcher;

/**
 * @class FileUtils
 * @brief Utility class providing static helper methods for file management
 * @author Wittefella, Michal Rost
 * @date 19.8.2012
 */
class FileUtils {

public:
    static bool removeRecurse(const QString &path, const QString &name);
    static void recurseFolder(const QString &path, const QString &parent,QStringList *list);
    static qint64 totalSize(const QList<QUrl> &files);
    static QStringList getApplicationNames();
    static QList<DesktopFile> getApplications();
    static QString getRealSuffix(const QString &name);
    static QIcon searchGenericIcon(const QString &category,const QIcon &defaultIcon = QIcon::fromTheme("unknown"));
    static QIcon searchMimeIcon(QString mime,const QIcon &defaultIcon = QIcon::fromTheme("unknown"));
    static QIcon searchAppIcon(const DesktopFile &app,const QIcon &defaultIcon = QIcon::fromTheme("application-x-executable"));

};

#endif // FILEUTILS_H
