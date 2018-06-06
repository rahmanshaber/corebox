/*
CoreArchiver is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; version 2
of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, see {http://www.gnu.org/licenses/}. */

#ifndef COREARCHIVER_H
#define COREARCHIVER_H

#include <QWidget>
#include <QLineEdit>

#include "libarchiveqt.h"


class corearchiver : public QWidget
{
    Q_OBJECT

public:
    corearchiver(QWidget *parent = 0);
    ~corearchiver();

    void compress(QStringList filePathList, QDir currentDir);
    void extract(QString archiveFilePath, QDir dest);

    QString archiveName, location, format, workingDir;
    QLineEdit *nameLE;
    QLineEdit *locationLE;
    QStringList filePathList;

private Q_SLOTS:
    void updateFileName( QString );
    void updateDirName();
    void updateFormat( QString );

};

#endif // COREARCHIVER_H
