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
#include <QGraphicsDropShadowEffect>
#include <QDirIterator>
#include <QProcess>
#include <QScreen>
#include <QMimeDatabase>
#include <QMimeType>
#include <QStyle>
#include <QStorageInfo>
#include <QDebug>
#include <QTextStream>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>

#include "../settings/settingsmanage.h"


struct SizeAndCountText
{
    QString sizeText ,countText;
};

enum sortOrder {
    ASCENDING = 0,
    DESCENDING = 1
};

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
    CorePDF,
    CoreTerminal
};

enum MessageType {
    Info = 0,
    Warning = 1,
    Tips = 2
};

enum FolderSetup {
    BookmarkFolder = 0,
    DriveMountFolder = 1,
    TrashFolder
};

 QStringList sortDate(QStringList &dateList, sortOrder s = ASCENDING);
 QStringList sortTime(QStringList &timeList, sortOrder s = ASCENDING, QString format = "hh.mm.ss");
 QStringList sortList(QStringList &list, sortOrder s = ASCENDING);
 QStringList sortDateTime(QStringList &dateTimeList, sortOrder s = ASCENDING);

 bool deleteLastLine(const QString &filePath);

 QString sentDateText(const QString &dateTime);
 bool checkRecentActivityFile();
 bool saveToRecent(QString appName, const QString &pathName);

 bool moveToTrash(const QString &fileName);
 void messageEngine(const QString &message, MessageType messageType);
 void openAppEngine(const QString &path);
 void setupFolder(FolderSetup fs);

 QRect screensize();

 AppsName nameToInt(QString appName);
 QIcon appsIcon(QString appName);

 QString formatSize(qint64 num);

 QString getCountText(const QString path);
 QString getMultipleCountText(const QStringList &paths);
 QString getMultipleFileSize(const QStringList &paths);
 QString getFileSize(const QString &path);

 SizeAndCountText getF(const QStringList &paths);
 qint64 getSize(const QString &path);

 QString checkIsValidDir(const QString str);
 QString checkIsValidFile(const QString str);

 QIcon geticon(const QString &filePath);
 QStringList fStringList(QStringList left, QStringList right, QFont font);

 QString getMountPathByName(const QString displayName);

 void getDirText(const QString &path);
 QString getStylesheetFileContent(const QString &path);
 QString readStringFromFile(const QString &path, const QIODevice::OpenMode &mode);
 QSettings *getStylesheetValue();

 static void addDropShadow(QWidget *widget, const int alpha, const int blur = 25, const QString stylesheet = "") {
     SettingsManage sm;
     if (sm.getAddShadow()) {
          QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(widget);
          effect->setBlurRadius(blur);
          effect->setColor(QColor(0, 0, 0, alpha));
          effect->setOffset(0);
          widget->setGraphicsEffect(effect);
          if (!stylesheet.isNull())
              widget->setStyleSheet(stylesheet);
     }
 }

#endif // GLOBALFUNCTIONS_H
