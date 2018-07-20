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

#ifndef COREBOX_H
#define COREBOX_H

#include <QWidget>
#include <QMainWindow>
#include <QMouseEvent>
#include <QCloseEvent>
#include <QPoint>
#include <QDir>
#include <QDateTime>
#include <QSettings>
#include <QPoint>

#include "settings/settingsmanage.h"
#include "bookmarks/bookmarks.h"
#include "bookmarks/bookmarkmanage.h"
#include "settings/settings.h"
#include "globalfunctions.h"


namespace Ui {
class CoreBox;
}

class CoreBox : public QMainWindow
{
    Q_OBJECT

public:
    explicit CoreBox(QWidget *parent = 0);
    ~CoreBox();

    void tabEngine(AppsName i, const QString arg = 0);
    void closeCurrentTab();
    bool eventFilter(QObject *obj, QEvent *evt);

protected:
    void closeEvent(QCloseEvent *event);
    void changeEvent(QEvent *event);

    bool isMouseDown = false ,isLeftDown = false ,isRightDown = false;
    QPoint mousePos, wndPos;

private slots:
    void on_windows_currentChanged(int index);
    void on_windows_tabCloseRequested(int index);
    void on_windows_tabBarClicked(int index);
    void on_maximizeButton_clicked();
    void on_restoreButton_clicked();
    void on_minimizeButton_clicked();
    void on_closeButton_clicked();
    void doubleClicked();
    void on_bookmarks_clicked();
    void on_dashboard_clicked();
    void on_start_clicked();
    void on_search_clicked();
    void on_corefm_clicked();
    void on_corepad_clicked();
    void on_box_clicked();
    void on_saveSession_clicked();

private:
    SettingsManage sm;
    Ui::CoreBox *ui;

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    int filterEngine(QString name);

};

#endif // COREBOX_H
