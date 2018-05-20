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

#ifndef FILEIO_H
#define FILEIO_H

#include "schedulecollection.h"
#include "schedule.h"
#include <QObject>
#include <QSettings>
#include <QList>

class FileIO : public QObject
{
    Q_OBJECT

public:
    explicit FileIO(QObject *parent = 0);
    static bool ExtractAudio();
    static bool DelExtracted();
    QList<Schedule *> LoadConfig();
    bool Save(ScheduleCollection*);
    static int LoadVolume();
    static void SaveVolume(int);
    static bool LoadWindowShow();
    static void SaveWindowShow(bool);
    static bool isMilTime();
    static void SaveTimeMode(bool isMilTime);
    static bool LoadWarnOnPm();
    static void SaveWarnOnPm(bool);

private:
    QSettings _Settings;
    QList<Schedule*> LegacyRead();

};

#endif // FILEIO_H
