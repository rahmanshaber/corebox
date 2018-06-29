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

#include "globalfunctions.h"
#include "corebox.h"

#include <QProcess>
#include <QScreen>
#include <QMimeDatabase>
#include <QMimeType>
#include <QStyle>
#include <QStorageInfo>
#include <QDebug>

#include <corefm/mimeutils.h>
#include "corepad/corepad.h"


bool moveToTrash(const QString &fileName) // moves a file or folder to trash folder
{
    if (getfilesize(fileName) >= 1073741824) {
        QMessageBox::StandardButton replyC;
        replyC = QMessageBox::warning(qApp->activeWindow(), "Warning!", "File size is about 1 GB or larger.\nPlease delete it instead of moveing to trash.\nDo you want to delete it?", QMessageBox::Yes | QMessageBox::No);
        if (replyC == QMessageBox::No) {
            return false;
        } else {
            QFile::remove(fileName);
            return true;
        }
    }
    else {
        QDir trash = QDir::home();
        trash.cd(".local/share/");
        trash.mkdir("Trash");
        trash.cd("Trash");
        trash.mkdir("files");
        trash.mkdir("info");

        QFile directorySizes(trash.path() + "/directorysizes");
        directorySizes.open(QFile::Append);

        QMessageBox::StandardButton reply;
        reply = QMessageBox::warning(qApp->activeWindow(), "Warning!", "Do you want to Trash the '" + fileName + "' ?", QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            QString fileLocation = fileName;
            if (QFile(fileLocation).exists()) {
                QFile(fileLocation).rename(trash.path() + "/files/" + QFileInfo(fileName).fileName());
            } else {
                QDir(QFileInfo(fileName).path()).rename(QFileInfo(fileName).fileName(), trash.path() + "/files/ " + QFileInfo(fileName).fileName());
            }
            QFile trashinfo(trash.path() + "/info/" + QFileInfo(fileName).fileName() + ".trashinfo");
            trashinfo.open(QFile::WriteOnly);
            trashinfo.write(QString("[Trash Info]\n").toUtf8());
            trashinfo.write(QString("Path=" + fileLocation + "\n").toUtf8());
            trashinfo.write(QString("DeletionDate=" + QDateTime::currentDateTime().toString("yyyy-MM-ddThh:mm:ss") + "\n").toUtf8());trashinfo.close();

            messageEngine("File Moved to Trash", MessageType::Info);
            return true;
        }
    }
    return false;
}

void setupFolder()
{
    qDebug() << "setupFolder";
    // Setup drive mount folder
    const QString d = QDir::homePath() + "/.coreBox";
    if(!QDir(d).exists()) {
        QDir::home().mkdir(".coreBox");
    }

    // Setup corebox folder for bookmarks
    const QString b = QDir::homePath() + ".config/coreBox";
    if (!QDir(b).exists()) {
        QDir::home().mkdir(".config/coreBox");
    }

    // Setup trash folder
    const QString t = QDir::homePath() + ".local/share/Trash";
    if (!QDir(t).exists()) {
        QDir trash = QDir::home();
        trash.cd(".local/share/");
        trash.mkdir("Trash");
        trash.cd("Trash");
        trash.mkdir("files");
        trash.mkdir("info");
    }
}

// ======================== Recent Activity =============================
QString sentDateText(const QString &dateTime) {
    // Can get error if date time format is not like this dd.MM.yyyy (28.06.2018)
    QDate temp = QDateTime::fromString(dateTime, "dd.MM.yyyy").date();
    int givenD = temp.day();
    int currnD = QDateTime::currentDateTime().toString("dd").toInt();
    int givenM = temp.month();
    int currnM = QDateTime::currentDateTime().toString("MM").toInt();
    int givenY = temp.year();
    int currnY = QDateTime::currentDateTime().toString("yyyy").toInt();

    if (givenY == currnY) {
        if (givenM == currnM) {
            if (givenD == currnD)
                return "Today";
            else if ((currnD - givenD) < 32)
                return QString("%1 day(s) ago").arg(currnD - givenD);
            else
                return QString("%1.%2.%3").arg(givenD, givenM, givenY);
        } else if ((currnM - givenM) < 13)
            return QString("%1 month(s) ago").arg(currnM - givenM);
        else
            return QString("%1.%2.%3").arg(givenD, givenM, givenY);
    } else
        return QString("%1.%2.%3").arg(givenD, givenM, givenY);

    return NULL;
}

bool checkRecentActivityFile() {
    QFile file(QDir::homePath() + "/.config/coreBox/RecentActivity");
    if (file.exists()) {
        return true;
    }
    return false;
}

bool saveToRecent(QString appName, const QString &pathName) // save file path and app name for recent activites
{
    SettingsManage sm;
    if (sm.getDisableRecent() == false) {
        if (appName.count() && pathName.count()) {
            QSettings recentActivity(QDir::homePath() + "/.config/coreBox/RecentActivity", QSettings::IniFormat);
            QDateTime currentDT = QDateTime::currentDateTime();
            QString group = currentDT.toString("dd.MM.yyyy");
            QString key = currentDT.toString("hh.mm.ss");
            recentActivity.beginGroup(group);
            recentActivity.setValue(key, appName + "\t\t\t" + pathName);
            recentActivity.endGroup();
            return true;
        }
    }
    return false;
}

// =========================================================================

void messageEngine(const QString &message, MessageType messageType) // engine show any message with type in desktop corner
{
    QLabel *l = new QLabel(message);
    QFont f ("Arial", 14, QFont::Bold);
    QWidget *mbox = new QWidget();
    QVBoxLayout *bi = new QVBoxLayout();
    QVBoxLayout *bii = new QVBoxLayout();
    QFrame *frame = new QFrame();
    frame->setStyleSheet("QFrame { border-radius: 10px; }");
    bii->addWidget(frame);
    frame->setLayout(bi);
    mbox->setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::ToolTip);
    mbox->setAttribute(Qt::WA_TranslucentBackground);
    mbox->setMinimumSize(230, 50);
    mbox->setLayout(bii);
    l->setStyleSheet("QLabel { padding: 10px; }");
    l->setFont(f);
    l->setAlignment(Qt::AlignCenter);
    bi->addWidget(l);
    bi->setContentsMargins(0, 0, 0, 0);
    QString stylesheet;
    if (messageType == MessageType::Info) {
        stylesheet = "QWidget { background-color: rgba(35, 35, 35, 150); color: #ffffff; border: 1px #2A2A2A; border-radius: 10px; }";
    } else if (messageType == MessageType::Warning) {
        stylesheet = "QWidget { background-color: rgba(240, 0, 0, 150); color: #ffffff; border: 1px #2A2A2A; border-radius: 10px; }";
    } else if (messageType == MessageType::Tips) {
        stylesheet = "QWidget { background-color: rgba(0, 0, 240, 150); color: #ffffff; border: 1px #2A2A2A; border-radius: 10px; }";
    } else {
        return;
    }

    mbox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    addDropShadow(mbox, 60, 25, stylesheet);

    mbox->show();

    int x = screensize().width() - (mbox->width() + 10);
    int y = screensize().height() - (mbox->height() + 10);
    mbox->move(x,y);

    QTimer::singleShot(3000, mbox, SLOT(close()));

}

AppsName nameToInt(QString appName)
{
    if (appName == "CoreFM" || appName == "corefm") {
        return CoreFM;
    } else if (appName == "CoreImage" || appName == "coreimage") {
        return CoreImage;
    } else if (appName == "CorePad" || appName == "corepad") {
        return CorePad;
    } else if (appName == "CorePaint" || appName == "corepaint") {
        return CorePaint;
    } else if (appName == "CorePlayer" || appName == "coreplayer") {
        return CorePlayer;
    } else if (appName == "DashBoard" || appName == "dashboard") {
        return Dashboard;
    } else if (appName == "Bookmarks" || appName == "bookmarks") {
        return Bookmarks;
    } else if (appName == "About" || appName == "about") {
        return About;
    } else if (appName == "Start" || appName == "start") {
        return StartView;
    } else if (appName == "Help" || appName == "help") {
        return Help;
    } else if (appName == "Settings" || appName == "settings") {
        return Settings;
    } else if (appName == "Search" || appName == "search") {
        return Search;
    } else if (appName == "CoreTime" || appName == "coretime") {
        return CoreTime;
    } else if (appName == "CoreBox" || appName == "corebox") {
        return Corebox;
    } else if (appName == "CoreRenamer" || appName == "corerenamer") {
        return CoreRenamer;
    } else if (appName == "CorePDF" || appName == "corepdf") {
        return CorePDF;
    } else if (appName == "CoreTerminal" || appName == "coreterminal") {
        return CoreTerminal;
    } else {
        return damn;
    }
}

QIcon appsIcon(QString appName)
{
    QString str = ":/icons/";

    if (appName == "DashBoard" || appName == "dashboard") {
        return QIcon(str + "DashBoard.svg");
    } else if (appName == "Bookmarks" || appName == "bookmarks") {
        return QIcon(str + "Bookmarks.svg");
    } else if (appName == "About" || appName == "about") {
        return QIcon(str + "About.svg");
    } else if (appName == "Start" || appName == "start") {
        return QIcon(str + "Start.svg");
    } else if (appName == "Search" || appName == "search") {
        return QIcon(str + "Search.svg");
    } else if (appName == "Help" || appName == "help") {
        return QIcon(str + "Help.svg");
    } else if (appName == "Settings" || appName == "settings") {
        return QIcon(str + "Settings.svg");
    } else if (appName == "CoreImage" || appName == "coreimage") {
        return QIcon(str + "CoreImage.svg");
    } else if (appName == "CorePad" || appName == "corepad") {
        return QIcon(str + "CorePad.svg");
    } else if (appName == "CorePaint" || appName == "corepaint") {
        return QIcon(str + "CorePaint.svg");
    } else if (appName == "CorePlayer" || appName == "coreplayer") {
        return QIcon(str + "CorePlayer.svg");
    } else if (appName == "CorePDF" || appName == "corepdf") {
        return QIcon(str + "CorePDF.svg");
    } else if (appName == "CoreTime" || appName == "coretime") {
        return QIcon(str + "CoreTime.svg");
    } else if (appName == "CoreFM" || appName == "corefm") {
        return QIcon(str + "CoreFM.svg");
    } else if (appName == "CoreTerminal" || appName == "coreterminal") {
        return QIcon(str + "CoreTerminal.svg");
    } else if (appName == "CoreRenamer" || appName == "corerenamer") {        
        return QIcon(str + "CoreRenemer.svg");
    } else if (!appName.isNull() || !appName.isEmpty()) {
        SettingsManage sm;
        return QIcon::fromTheme(appName, QIcon::fromTheme(sm.getThemeName()));
    } else {
        return QIcon();
    }
}

QString formatSize(qint64 num) // separete size in universal size format
{
    QString total;
    const qint64 kb = 1024;
    const qint64 mb = 1024 * kb;
    const qint64 gb = 1024 * mb;
    const qint64 tb = 1024 * gb;

    if (num >= tb) total = QString("%1TB").arg(QString::number(qreal(num) / tb, 'f', 2));
    else if(num >= gb) total = QString("%1GB").arg(QString::number(qreal(num) / gb, 'f', 2));
    else if(num >= mb) total = QString("%1MB").arg(QString::number(qreal(num) / mb, 'f', 1));
    else if(num >= kb) total = QString("%1KB").arg(QString::number(qreal(num) / kb,'f', 1));
    else total = QString("%1 bytes").arg(num);

    return total;
}

qint64 getfilesize(QString path) //get size of single file in int
{
    QProcess p;
    QString commd = "du -sb --total \"" + path + "\"";
    p.start(commd.toLatin1());
    p.waitForFinished();
    QString output(p.readAllStandardOutput());
    return output.split("\n").at(1).split("	").at(0).toLongLong();
}

QString getFileSize(const QString path) //get size of single file in string
{
    return formatSize(getfilesize(path));
}

QString getMultipleFileSize(const QStringList paths) // get file size of multiple files
{
    QString pathNames;
    for (int i = 0; i < paths.count(); i++) {
        pathNames = pathNames + " \"" + paths.at(i) + "\"";
    }
    QProcess p;
    QString commd = "du -sb --total " + pathNames;
    p.start(commd.toLatin1());
    p.waitForFinished();
    QString output(p.readAllStandardOutput());
    QStringList l = output.split("\n");

    return formatSize(l.at(l.count() - 2).split("\t").at(0).toLongLong());
}

void openAppEngine(const QString &path) // engine send right file to coreapps or system
{
    CoreBox *cBox = qobject_cast<CoreBox*>(qApp->activeWindow());
    QFileInfo info(path);
    if(!info.exists() && !path.isEmpty()){
        messageEngine("File not exists", MessageType::Warning);
        return;
    }

    QStringList image, txts,pdf;
    image << "jpg" << "jpeg" << "png" << "bmp" << "ico" << "svg" << "gif";
    txts << "txt" << "pro" << "";
    pdf << "pdf";

    QString suffix = QFileInfo(path).suffix();

    //CoreFM
    if (info.isDir()) {
        cBox->tabEngine(CoreFM, info.absoluteFilePath());
        return;
    }

    //CoreImage
    else if (image.contains(suffix, Qt::CaseInsensitive)) {
        cBox->tabEngine(CoreImage, info.absoluteFilePath());
        return;
    }

    //CorePad
    else if (txts.contains(suffix, Qt::CaseInsensitive)) {
        cBox->tabEngine(CorePad, info.absoluteFilePath());
        return;
    }

    //CorePDF
    else if (pdf.contains(suffix, Qt::CaseInsensitive)) {
        cBox->tabEngine(CorePDF, info.absoluteFilePath());
        return;
    }

    //sendtomimeutils
    else {
        QFileInfo p(path);
        MimeUtils *m = new MimeUtils(cBox);
        m->openInApp(p,cBox);
    }
}

QString checkIsValidDir(QString str) // cheack if a folder/dir is valid
{
    if (str.isEmpty() || str.isNull()) {
        return NULL;
    } else {
        QFileInfo dir(str);
        if (dir.isDir()) {
            if (dir.isRoot()) return str;
            else {
                if (str.endsWith('/')) return str.remove(str.length() - 1, 1);
                else return str;
            }
        }
    }
    return NULL;
}

QString checkIsValidFile(const QString str) // cheack if a file is valid
{
    if (str.isEmpty() || str.isNull()) {
        return NULL;
    } else {
        QFileInfo fi(str);
        if (fi.isFile()) {
            return str;
        }
    }
    return NULL;
}

QRect screensize() // gives the system screen size
{
    QScreen * screen = QGuiApplication::primaryScreen();
    return screen->availableGeometry();
}

QIcon geticon(const QString &filePath) // gives a file or folder icon from system
{
    QIcon icon;
    QFileInfo info(filePath);

    QMimeDatabase mime;
    QMimeType mType;

    mType = mime.mimeTypeForFile(filePath);
    icon = QIcon::fromTheme(mType.iconName());

    if (icon.isNull())
        return QApplication::style()->standardIcon(QStyle::SP_FileIcon);
    else
        return icon;
}

QStringList fStringList(QStringList left, QStringList right, QFont font) // add two stringlist with ":"
{
    QFontMetrics *fm = new QFontMetrics(font);
    int large = 0;

    for (int i = 0; i < left.count(); i++) {
        if (large < fm->width(left.at(i))) {
            large = fm->width(left.at(i));
        }
    }

    large = large + fm->width('\t');

    for (int i = 0; i < left.count(); i++) {
        while (large >= fm->width(left.at(i))) {
             left.replace(i, QString(left.at(i) + QString('\t')));
        }
    }

    for (int i = 0; i < left.count(); i++) {
        QString total = left.at(i);
        QString firstWoard = total.at(0).toUpper();
        QString otherWord = total.right(total.length() - 1 );
        QString s = left.at(i);
        left.replace(i, firstWoard + otherWord + ": " + right.at(i));
    }

    return left;
}

QString getMountPathByName(const QString displayName) // get mount path by partition display name
{
    if(displayName.isNull() || displayName.isEmpty()) return NULL;

    else {
        const auto allMounted = QStorageInfo::mountedVolumes();
        for(auto& singleMounted : allMounted){
            if (singleMounted.displayName() == displayName) return singleMounted.rootPath();
        }
    }
    return NULL;
}

#include <QDirIterator>

qint64 getF(QStringList paths)
{
    qint64 totalSize = 0;
    Q_FOREACH( QString path, paths ) {
//        if ( *terminate )
//            return;

        if ( QFileInfo( path ).isDir() ) {
            //recurseProperties( path );
            QDirIterator it( path, QDir::AllEntries | QDir::System | QDir::NoDotAndDotDot | QDir::NoSymLinks | QDir::Hidden, QDirIterator::Subdirectories );
            while ( it.hasNext() ) {
//                if ( *terminate )
//                    return;

                it.next();
                if( it.fileInfo().isDir() ) {
                    if ( it.filePath() == path )
                        continue;

                    //folders++;

                    //if( folders % 32 == 0 ) {
                        //emit updateSignal();
                    //}
                }

                else {
                    //files++;
                    totalSize += it.fileInfo().size();
                }
            }
        }

        else {
            //files++;
            totalSize += getSize( path );
        }
    }

    qDebug() << "SIZE : " << totalSize;
    //emit updateSignal();
    return totalSize;
}

#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>

qint64 getSize(QString path)
{

    struct stat statbuf;
    if ( stat( path.toLocal8Bit().constData(), &statbuf ) != 0 )
        return 0;

    switch( statbuf.st_mode & S_IFMT ) {
        case S_IFREG: {

            return statbuf.st_size;
        }

        case S_IFDIR: {
            DIR* d_fh;
            struct dirent* entry;
            QString longest_name;

            while ( ( d_fh = opendir( path.toLocal8Bit().constData() ) ) == NULL ) {
                qWarning() << "Couldn't open directory:" << path;
                return statbuf.st_size;
            }

            quint64 size = statbuf.st_size;

            longest_name = QString( path );
            if ( not longest_name.endsWith( "/" ) )
                longest_name += "/";

            while( ( entry = readdir( d_fh ) ) != NULL ) {

                /* Don't descend up the tree or include the current directory */
                if ( strcmp( entry->d_name, ".." ) != 0 && strcmp( entry->d_name, "." ) != 0 ) {

                    if ( entry->d_type == DT_DIR ) {

                        /* Recurse into that folder */
                        size += getSize( longest_name + entry->d_name );
                    }

                    else {

                        /* Get the size of the current file */
                        size += getSize( longest_name + entry->d_name );
                    }
                }
            }

            closedir( d_fh );
            return size;
        }

        default: {

            /* Return 0 for all other nodes: chr, blk, lnk, symlink etc */
            return 0;
        }
    }

    /* Should never come till here */
    return 0;
}
