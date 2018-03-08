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

#ifndef START_H
#define START_H

#include <QWidget>
#include <QResizeEvent>
#include <QListWidgetItem>
#include <QTableWidgetItem>

#include "../settings/settingsmanage.h"
#include "corebox/corebox.h"
#include "bookmarks/bookmarkmanage.h"
#include "corebox/globalfunctions.h"


namespace Ui {
class Start;
}

class Start : public QWidget
{
    Q_OBJECT

public:
    explicit Start(QWidget *parent = 0);
    ~Start();
    void loadsettings();

private slots:
    void on_cleaRecent_clicked();
    void on_recents_itemDoubleClicked(QTableWidgetItem *item);
    void on_reloadR_clicked();
    void on_reloadB_clicked();
    void on_appCollect_itemDoubleClicked(QListWidgetItem *item);

    void resizeEvent(QResizeEvent *event);

private:
    SettingsManage sm;
    Ui::Start *ui;

};

#endif // START_H
