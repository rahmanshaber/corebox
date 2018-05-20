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

#ifndef SCHEDULECOLLECTION_H
#define SCHEDULECOLLECTION_H

#include "schedule.h"

#include <QObject>
#include <QList>


class ScheduleCollection : public QObject
{
    Q_OBJECT

public:
    explicit ScheduleCollection(QObject *parent = 0);
    Schedule* GetSchedule(int);
    QList<Schedule*> GetScheduleList();
    int GetCount();
    void AddSchedule(Schedule*);
    void removeScheduleByIndex(int);
    void LoadSchedules();
    void Save();

private:
    QList<Schedule*> _Collection;

};

#endif // SCHEDULECOLLECTION_H
