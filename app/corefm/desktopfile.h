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

#ifndef DESKTOPFILE_H
#define DESKTOPFILE_H

#include "properties.h"

#include <QStringList>
#include <QFile>


/**
 * @class DesktopFile
 * @brief Represents a linux desktop file
 * @author Michal Rost
 * @date 13.1.2013
 */
class DesktopFile {

public:
    explicit DesktopFile(const QString &fileName = "");
    QString getFileName() const;
    QString getPureFileName() const;
    QString getName() const;
    QString getExec() const;
    QString getIcon() const;
    QString getType() const;
    QStringList getCategories() const;
    QStringList getMimeType() const;

private:
    QString fileName;
    QString name;
    QString exec;
    QString icon;
    QString type;
    QStringList categories;
    QStringList mimeType;

};

#endif // DESKTOPFILE_H
