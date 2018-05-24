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


#ifndef GLOBALFUNCTIONS_H
#define GLOBALFUNCTIONS_H

#include <QApplication>
#include <QDateTime>
#include <QLabel>
#include <QVBoxLayout>
#include <QDesktopWidget>
#include <QTimer>
#include <QFile>
#include <QDir>
#include <QFont>
#include <QFontMetrics>
#include <QMessageBox>
#include <QFileInfo>
#include <QSettings>

#include "../settings/settingsmanage.h"

enum AppsName {
    damn = -1,
    CoreFM = 0,
    CoreImage,
    CorePad,
    CorePaint,
    CorePlayer,
    Dashboard,
    Bookmarks,
    About,
    StartView,
    Help,
    Settings,
    Search,
    CoreTime,
    CoreRenamer,
    Corebox,
    CorePDF
};

 bool moveToTrash(QString fileName);
 bool saveToRecent(QString appName, QString pathName);
 bool saveToRecent(QString appName, QString pathName, QString iconPath);
 void messageEngine(QString message, QString messageType);
 void openAppEngine(QString path);

 QRect screensize();

 AppsName nameToInt(QString appName);
 QIcon appsIconPath(QString appName);

 QString formatSize(qint64 num);

 QString getFileSize(QString path);
 QString getMultipleFileSize(QStringList paths);

 qint64 getfilesize(QString path);

 QString checkIsValidDir(QString str);
 QString checkIsValidFile(QString str);

 QIcon geticon(const QString &filePath);
 QStringList fStringList(QStringList left, QStringList right, QFont font);

 QString getMountPathByName(QString displayName);


#endif // GLOBALFUNCTIONS_H
