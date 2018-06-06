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

#include <QWidget>
#include <QFocusEvent>
#include <QCloseEvent>
#include <QDir>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QTimer>

#include "../settings/settingsmanage.h"
#include "../corebox/globalfunctions.h"
#include "../corebox/corebox.h"
#include "about/about.h"
#include "corescreenshot/mainwindow.h"
#include "dashboard/upower.h"
#include "dashboard/battery.h"
#include "dashboard/lib/info_manager.h"


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
    void collectNotes();
    void sysWsetup();
    void networkWsetup();
    void widgetList();

public slots:
    void ShowWindow(QSystemTrayIcon::ActivationReason Reason);

private slots:
    //void on_percentage_clicked();

    void on_one_clicked();
    void on_two_clicked();
    void on_three_clicked();
    void on_four_clicked();
    void on_five_clicked();
    void on_six_clicked();
    void on_seven_clicked();
    void on_eight_clicked();
    void on_nine_clicked();
    void on_zero_clicked();
    void on_dot_clicked();
    void on_equal_clicked();
    void on_clear_clicked();
    void on_multiply_clicked();
    void on_subtract_clicked();
    void on_add_clicked();
    void on_division_clicked();

    void showTime();
    void on_hide_clicked();
    void coreBoxAbout();

    void on_corepad_clicked();
    void on_screenshot_clicked();
    void on_corepaint_clicked();
    void on_corefm_clicked();
    void on_start_clicked();
    void on_search_clicked();

    void actionshow();
    void batteryCheck();

};

#endif // COREACTION_H
