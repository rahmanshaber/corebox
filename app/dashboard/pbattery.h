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

#ifndef PBATTERY_H
#define PBATTERY_H

#include "upower.h"
#include "battery.h"

#include <QWidget>
#include <QMetaProperty>
#include <QStringListModel>

#include "corebox/globalfunctions.h"


namespace Ui {
class pBattery;
}

class pBattery : public QWidget
{
    Q_OBJECT

public:
    explicit pBattery(QWidget *parent = 0);
    ~pBattery();

    void reload();

private slots:
    void on_batteriesList_currentIndexChanged(int index);
    void on_refresh_clicked();

private:
    Ui::pBattery *ui;
    UPower *m_upower;
    Battery *m_model;
    QStringList batteries;

    void setupBatteryPage();
};

#endif // PBATTERY_H
