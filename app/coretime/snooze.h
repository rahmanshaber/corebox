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

#ifndef SNOOZE_H
#define SNOOZE_H

#include "alarm.h"

#include <QDialog>
#include <QTimer>


namespace Ui {
class snooze;
}

class snooze : public QDialog
{
    Q_OBJECT

public:
    explicit snooze(QWidget *parent = 0,Alarm * curAlarm=0);
    ~snooze();
    bool isDismissed;

public slots:
    void UpdateClock();

private slots:
    void DismissClicked();
    void SnoozeClicked();
    void timerOut();

private:
    Ui::snooze *ui;
    void SetupClock();
    Alarm *_curAlarm;
    QTimer *_snoozeTimer;

};

#endif // SNOOZE_H
