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

#ifndef COREFM_H
#define COREFM_H

#include "mymodel.h"
#include "progressdlg.h"
#include "propertiesw.h"
#include "tabbar.h"
#include "fileutils.h"
#include "mimeutils.h"

#include <QWidget>
#include <QSettings>
#include <QTreeView>
#include <QSortFilterProxyModel>
#include <QSignalMapper>
#include <QFileSystemWatcher>
#include <QMouseEvent>
#include <QListView>
#include <QAbstractItemView>

#include "udisks2.h"
#include "../corebox/corebox.h"
#include "../settings/settingsmanage.h"
#include "../corebox/globalfunctions.h"
#include "../bookmarks/bookmarks.h"
#include "../bookmarks/bookmarkdialog.h"


namespace Ui {
class corefm;
}

class corefm : public QWidget
{
    Q_OBJECT

public:
    explicit corefm(QWidget *parent = 0);
    ~corefm();
    myModel *modelList;

    Ui::corefm *ui;

    CoreBox& corebox() {
        return corebox_;
    }

    void goTo(const QString path);

protected:
    void closeEvent(QCloseEvent *event);

public slots:
    void treeSelectionChanged(QModelIndex,QModelIndex);
    void listSelectionChanged(const QItemSelection, const QItemSelection);
    void listDoubleClicked(QModelIndex);
    void lateStart();

    bool cutCopyFile(const QString &source, QString dest, qint64 totalSize, bool cut);
    bool pasteFiles(const QList<QUrl> &files, const QString &newPath, const QStringList &cutList);
    bool linkFiles(const QList<QUrl> &files, const QString &newPath);
    bool copyFolder(const QString &srcFolder, const QString &dstFolder, qint64, bool);

    void pathEditChanged(QString);
    void executeFile(QModelIndex, bool);
    void clipboardChanged();
    void toggleSortBy(QAction* action);
    void setSortColumn(QAction *columnAct);
    void actionMapper(QString);
    void dragLauncher(const QMimeData *data, const QString &newPath, myModel::DragMode dragMode);
    void pasteLauncher(const QMimeData *data, const QString &newPath, const QStringList &cutList);
    void pasteLauncher(const QList<QUrl> &files, const QString &newPath, const QStringList &cutList, bool link = false);
    void progressFinished(int,QStringList);
    void listItemClicked(QModelIndex);
    void listItemPressed(QModelIndex);
    void tabChanged(int index);
    int addTab(const QString path);
    void clearCutItems();
    void zoomInAction();
    void zoomOutAction();
//    void focusAction();
    void dirLoaded();
    void thumbUpdate(QModelIndex);
    void addressChanged(int,int);
    void loadSettings();
    void pressed();

signals:
    void updateCopyProgress(qint64, qint64, QString);
    void copyProgressFinished(int,QStringList);

private slots:
    void selectApp();
    void openInApp();
    void on_viewlist_customContextMenuRequested(const QPoint &pos);
    void on_viewtree_customContextMenuRequested(const QPoint &pos);
    void on_actionSelectAll_triggered();
    void on_Tools_clicked(bool checked);
    void on_actionCorePlayer_triggered();
    void on_actionCorePad_triggered();
    void on_actionCoreFM_triggered();
    void on_actionCoreImage_triggered();
    void on_actionCorePaint_triggered();
    void on_actionCoreBox_triggered();
    void on_actionCorePDF_triggered();
    void on_actionRename_triggered();
    void on_actionOpen_triggered();
    void on_actionDelete_triggered();
    void on_actionBack_triggered();
    void on_actionUp_triggered();
    void on_actionCut_triggered();
    void on_actionCopy_triggered();
    void on_actionPaste_triggered();
    void on_actionProperties_triggered();
    void on_actionRefresh_triggered();
    void on_actionNewFolder_triggered();
    void on_actionNewTextFile_triggered();
    void on_actionNewTab_triggered();
    void on_actionTerminal_triggered();
    void on_actionShowThumbnails_triggered();
    void on_actionAscending_triggered(bool checked);
    void on_SHome_clicked();
    void on_SDesktop_clicked();
    void on_SDownloads_clicked();
    void on_icon_clicked(bool checked);
    void on_detaile_clicked(bool checked);
    void on_actionTrash_it_triggered();
    void on_showHidden_clicked(bool checked);
    void on_showthumb_clicked(bool checked);
    void on_SBookMarkIt_clicked();
    void on_searchHere_clicked();
    void on_actionExtract_Here_triggered();
    void on_actionCreate_Archive_triggered();
    void on_STrash_clicked();
    void on_emptyTrash_clicked();
    void on_partitions_itemClicked(QListWidgetItem *item);
    void blockDevicesChanged();
    void on_actionDesktop_triggered();
    void on_actionHome_triggered();
    void on_actionRun_triggered();
    void sendToPath();



private:
    SettingsManage sm;
    void writeSettings();
    void shotcuts();

    void recurseFolder(const QString path, QString parent, QStringList *);//delete
    int showReplaceMsgBox(const QFileInfo &f1, const QFileInfo &f2);
    QMenu* createOpenWithMenu();
    QMenu *globalmenu();
    QMenu *sendto();

    int zoom;
    int zoomTree;
    int zoomList;
    int zoomDetail;
    int currentView;        // 0=list, 1=icons, 2=details
    int currentSortColumn;  // 0=name, 1=size, 3=date
    Qt::SortOrder currentSortOrder;

    QCompleter *customComplete;

    MimeUtils *mimeUtils;
    tabBar *tabs;
    myProgressDialog * progress;
    propertiesw * properties;
    QSettings *sett;

    QTreeView *tree;

    QString term;
    QFileInfo curIndex;
    QModelIndex backIndex;

    QSortFilterProxyModel *modelTree;
    QSortFilterProxyModel *modelView;

    QItemSelectionModel *treeSelectionModel;
    QItemSelectionModel *listSelectionModel;
    QSocketNotifier *notify;

    QStringList mounts;
    QString startPath;

    QList<QIcon> *actionIcons;
    QList<QAction*> *actionList;

    QActionGroup *sortByActGrp;
    QAction *sortNameAct;
    QAction *sortDateAct;
    QAction *sortSizeAct;
    QAction *sortAscAct;
    QAction *focusAddressAct;
    QAction *focusTreeAct;
    QAction *focusListAct;

    QFileSystemWatcher *watcher;

    QString selcitempath;
    int selcitem;

    CoreBox corebox_;
    UDisks2 *udisks;

};

//---------------------------------------------------------------------------------
// Subclass QSortFilterProxyModel and override 'filterAcceptsRow' to only show
// directories in tree and not files.
//---------------------------------------------------------------------------------
class mainTreeFilterProxyModel : public QSortFilterProxyModel
{
protected:
    virtual bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const;
};


//---------------------------------------------------------------------------------
// Subclass QSortFilterProxyModel and override 'lessThan' for sorting in list/details views
//---------------------------------------------------------------------------------
class viewsSortProxyModel : public QSortFilterProxyModel
{
protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
};


//---------------------------------------------------------------------------------
// Subclass QCompleter so we can use the SortFilterProxyModel above for the address bar.
//---------------------------------------------------------------------------------
class myCompleter : public QCompleter
{
public:
    QString pathFromIndex(const QModelIndex& index) const;
    QStringList splitPath(const QString& path) const;
};

#endif // COREFM_H
