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

#include "timer.h"
#include "schedule.h"
#include "schedulecollection.h"
#include "fileio.h"

#include <QTimer>
#include <QDateTime>
#include <QDate>


Timer::Timer(QObject *parent,ScheduleCollection *Collection) :
    QObject(parent)
{
    this->_Schedules=Collection;
}

void Timer::StartTimer(Alarm *MainAlarm)
{
    this->_CurAlarm=MainAlarm;
    QTimer *timer=new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(AlarmCheck()));
    timer->start(1000);
}

void Timer::AlarmCheck()
{
    //Compare saved times with now time
    if(!this->_CurAlarm->isPlaying() && this->_CurAlarm->canResume)
    {
        Schedule *cur_sche;
        foreach(cur_sche,this->_Schedules->GetScheduleList())
        {
            if(cur_sche->GetCustomSoundEnabled())
            {
                this->_CurAlarm->SetCustomPath(cur_sche->GetCustomSound());
            }

            QDateTime RightNow=QDateTime::currentDateTime();//We're in now, now...
            bool soundAlarm=false;

            if(cur_sche->GetTime().hour()==RightNow.time().hour() && cur_sche->GetTime().minute()==RightNow.time().minute())
            {
                switch(RightNow.date().dayOfWeek())
                {
                //WeekDay Alarms
                case 1:
                    if(cur_sche->isMonEnabled())
                    {
                        soundAlarm=true;
                    }
                    break;

                case 2:
                    if(cur_sche->isTueEnabled())
                    {
                        soundAlarm=true;
                    }
                    break;

                case 3:
                    if(cur_sche->isWedEnabled())
                    {
                        soundAlarm=true;
                    }
                    break;

                case 4:
                    if(cur_sche->isThurEnabled())
                    {
                        soundAlarm=true;
                    }
                    break;

                case 5:
                    if(cur_sche->isFriEnabled())
                    {
                        soundAlarm=true;
                    }
                    break;
                case 6:
                    if(cur_sche->isSatEnabled())
                    {
                        soundAlarm=true;
                    }
                    break;
                case 7:
                    if(cur_sche->isSunEnabled())
                    {
                        soundAlarm=true;
                    }
                    break;

                }
            }
            //Check for Custom Date Alarms
            if(cur_sche->GetCustomEnabled() && cur_sche->GetCustomDate()==RightNow.date() &&
                    cur_sche->GetTime().minute()==RightNow.time().minute()
                    && cur_sche->GetTime().hour()==RightNow.time().hour())
            {
                soundAlarm=true;
            }

            if(soundAlarm)
            {
                //Set Condtion One!
                this->_CurAlarm->Start(cur_sche->GetCustomSoundEnabled());
            }
        }
    }
}

