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

#ifndef COREARCHIVER_H
#define COREARCHIVER_H

#include <QWidget>
#include <QFileDialog>

#include "libarchiveqt.h"

namespace Ui {
class corearchiver;
}

class corearchiver : public QWidget
{
    Q_OBJECT

public:
    explicit corearchiver(QWidget *parent = 0);
    ~corearchiver();

    void compress(QStringList filePathList, QDir currentDir);
    void extract(QString archiveFilePath, QDir dest);

    QString archiveName, location, format, workingDir;
    QStringList filePathList;

    void setFilename(QString fileName);
    void setFolderPath(QString path);

private Q_SLOTS:
    void updateFileName( QString );
    void updateDirName();
    void updateFormat( QString );

private:
    Ui::corearchiver *ui;
    void startsetup();

};

#endif // COREARCHIVER_H
