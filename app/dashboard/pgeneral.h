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

#ifndef PGENERAL_H
#define PGENERAL_H

#include <QWidget>
#include <QStringListModel>
#include <QSysInfo>
#include <QtNetwork>
#include <QFontMetrics>
#include <QFont>
#include <QObject>

#include "dashboard/info_manager.h"
#include "dashboard/circlebar.h"
#include "Info/system_info.h"
#include "corebox/globalfunctions.h"


namespace Ui {
class pgeneral;
}

class pgeneral : public QWidget
{
    Q_OBJECT

public:
    explicit pgeneral(QWidget *parent = 0);
    ~pgeneral();

private slots:
    void init();
    void systemInformationInit();
    void updateCpuBar();
    void updateMemoryBar();
    void updateNetworkBar();

private:
    Ui::pgeneral *ui;

    CircleBar* cpuBar;
    CircleBar* memBar;

    QTimer *timer;
    InfoManager *im;

    QString cpuCore,cpuModel,cpuSpeed,username;

};

#endif // PGENERAL_H
