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

#ifndef COREACTION_H
#define COREACTION_H

#include "wcalender.h"
#include "wsystem.h"
#include "wnetwork.h"
#include "wcalculator.h"
#include "wnotes.h"
#include "wbattery.h"

#include <QWidget>
#include <QFocusEvent>
#include <QCloseEvent>
#include <QDir>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QDesktopWidget>
#include <QDateTime>
#include <QTimer>
#include <QFile>
#include <QDoubleValidator>

#include "settings/settingsmanage.h"
#include "corebox/globalfunctions.h"
#include "corebox/corebox.h"
#include "about/about.h"
#include "dashboard/info_manager.h"
#include "dashboard/upower.h"
#include "dashboard/battery.h"
#include "coreshot/coreshotdialog.h"


namespace Ui {
class coreaction;
}

class coreaction : public QWidget
{
    Q_OBJECT

public:
    explicit coreaction(QWidget *parent = 0);
    ~coreaction();

    QSystemTrayIcon *trayIcon;
    void focusOutEvent(QFocusEvent *event);
    void loadsettings();

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::coreaction *ui;
    SettingsManage sm;
    QMenu *trayIconMenu;
    QDir currentDir;
    QTimer *timer;
    InfoManager *im;

    void widget();
    void tryicon();
    void widgetList();
    bool checkWTime();
    bool checkWCalendar();
    bool checkWSystem();
    bool checkWNetwork();
    bool checkWCalculator();
    bool checkWNotes();
    bool checkWBattery();

public slots:
    void ShowWindow(QSystemTrayIcon::ActivationReason Reason);

private slots:
    void showTime();
    void coreBoxAbout();
    void actionshow();
    void on_corepad_clicked();
    void on_screenshot_clicked();
    void on_corepaint_clicked();
    void on_corefm_clicked();
    void on_start_clicked();
    void on_search_clicked();

};

#endif // COREACTION_H
