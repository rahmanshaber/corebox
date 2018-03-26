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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QTreeWidgetItem>
#include <QPushButton>

#include "settingsmanage.h"
#include "corefm/mimeutils.h"
#include "corefm/fileutils.h"
#include "corefm/applicationdialog.h"
#include "../corebox/corebox.h"
#include "../corebox/globalfunctions.h"


namespace Ui {
class settings;
}

class settings : public QWidget
{
    Q_OBJECT

public:
    explicit settings(QWidget *parent = 0);
    ~settings();

    QString selectedTerminal;

protected slots:
    //void getIcon(QTreeWidgetItem *item, int column);
    //void onActionChanged(QTreeWidgetItem *item, int column);
    void onMimeSelected(QTreeWidgetItem* current, QTreeWidgetItem* previous);
    void updateMimeAssoc(QTreeWidgetItem* item);
    void showAppDialog();
    void removeAppAssoc();
    void moveAppAssocUp();
    void moveAppAssocDown();

private slots:
    void on_cancel_clicked();
    void on_ok_clicked();
    void on_terminals_currentIndexChanged(const QString &arg1);
    void on_browseF_clicked();
    void on_browseS_clicked();
    void pageClick(QPushButton *btn, int i, QString title);
    void on_corebox_clicked();
    void on_coreaction_clicked();
    void on_corefm_clicked();
    void on_corescrshot_clicked();

private:
    Ui::settings *ui;
    SettingsManage sm;

};

#endif // SETTINGS_H
