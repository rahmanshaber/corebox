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
 void messageEngine(QString message, QString messageType);
 void openAppEngine(QString path);

 AppsName nameToInt(QString appName);
 QString appsIconPath(QString appName);

 QString formatSize(qint64 num);

 QString getFileSize(QString path);
 QString getMultipleFileSize(QStringList paths);

 qint64 getfilesize(QString path);
 qint64 getmultiplefilesize(QStringList paths);


#endif // GLOBALFUNCTIONS_H
