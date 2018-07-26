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

#include "schedulecollection.h"
#include "schedule.h"
#include "fileio.h"

#include <QString>
#include <QSettings>


ScheduleCollection::ScheduleCollection(QObject *parent) :
    QObject(parent){}

void ScheduleCollection::LoadSchedules()
{
    FileIO *config=new FileIO(this);
    this->_Collection = config->LoadConfig();
}

Schedule* ScheduleCollection::GetSchedule(int Index)
{
    return _Collection[Index];
}

void ScheduleCollection::Save()
{
    FileIO * configIO=new FileIO(this);
    configIO->Save(this);
}

QList<Schedule*> ScheduleCollection::GetScheduleList()
{
    return this->_Collection;
}

int ScheduleCollection::GetCount()
{
    return this->_Collection.count();
}

void ScheduleCollection::AddSchedule(Schedule *scheToAdd)
{
    this->_Collection.append(scheToAdd);
}

void ScheduleCollection::removeScheduleByIndex(int index)
{
    this->_Collection.removeAt(index);
}
