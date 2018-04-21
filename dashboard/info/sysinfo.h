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

#ifndef SYSINFO_H
#define SYSINFO_H

#include "circlebar.h"

#include <QWidget>

#include "../lib/info_manager.h"


namespace Ui {
class sysinfo;
}

class sysinfo : public QWidget
{
    Q_OBJECT

public:
    explicit sysinfo(QWidget *parent = 0);
    ~sysinfo();

private slots:
    void init();
    void systemInformationInit();
    void updateCpuBar();
    void updateMemoryBar();
    void updateNetworkBar();

private:
    Ui::sysinfo *ui;

    CircleBar* cpuBar;
    CircleBar* memBar;

    QTimer *timer;
    InfoManager *im;

    QString cpuCore;
    QString cpuModel;
    QString cpuSpeed;
    QString username;

};

#endif // SYSINFO_H
