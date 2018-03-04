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

#ifndef DASHBOARD_H
#define DASHBOARD_H

#include "upower.h"
#include "battery.h"

#include <QListWidgetItem>
#include <QWidget>
#include <QPainter>
#include <QFile>
#include <QProcess>
#include <sys/vfs.h>
#include <sys/stat.h>

#include "corefm/udisks2.h"
#include "resources/resources_page.h"
#include "info/sysinfo.h"
#include "globalfunctions.h"

QString formatSize(qint64);

class UDisks2;
namespace Ui {
class dashboard;
}


class dashboard : public QWidget
{
    Q_OBJECT

private slots:
    void on_drives_currentTextChanged(const QString &currentText);
    void on_blocks_currentTextChanged(const QString &currentText);
    void udisks2_blockAdded(const QString &node);
    void udisks2_blockRemoved(const QString &node);
    void udisks2_blockChanged(const QString &node);
    void udisks2_driveAdded(const QString &node);
    void udisks2_driveRemoved(const QString &node);
    void udisks2_driveChanged(const QString &node);
    void udisks2_filesystemAdded(const QString &node);
    void udisks2_filesystemRemoved(const QString &node);
    void udisks2_filesystemChanged(const QString &node);
    void on_batteriesList_currentIndexChanged(int index);
    void on_refresh_clicked();
    void on_mount_2_clicked();
    void on_unmount_2_clicked();
    void pageClick(QPushButton *btn, int i, QString title);
    void on_Bresource_clicked();
    void on_Bdrives_clicked();
    void on_Bbattery_clicked();
    void on_Bgeneral_clicked();

    void on_blocks_itemSelectionChanged();


    void on_drives_itemSelectionChanged();

public:
    explicit dashboard(QWidget *parent = 0);
    ~dashboard();
    QString getDriveInfo(QString);
    UPower *m_upower;
    Battery *m_model;
    QStringList batteries;

private:
    Ui::dashboard *ui;
    UDisks2 *disks;
    sysinfo *info;
    ResourcesPage *resource;

};

#endif // DASHBOARD_H
