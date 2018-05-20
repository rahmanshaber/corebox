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

#include "snooze.h"
#include "alarm.h"
#include "ui_snooze.h"

#include <QTimer>
#include <QTime>
#include <QDesktopWidget>


snooze::snooze(QWidget *parent,Alarm *curAlarm) :
    QDialog(parent),
    ui(new Ui::snooze)
{
    ui->setupUi(this);
    this->_curAlarm=curAlarm;
    this->_snoozeTimer=new QTimer(this);
    this->isDismissed=false;
    SetupClock();

    QDesktopWidget w;
    w.width();
    this->setFixedWidth(w.width());
    this->setFixedHeight(w.height()/2);
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    this->move(qApp->desktop()->availableGeometry(this).center()-rect().center());

    //setup connections
    connect(ui->snzBtn,SIGNAL(clicked()),SLOT(SnoozeClicked()));
    connect(ui->DismissBtn,SIGNAL(clicked()),SLOT(DismissClicked()));
    connect(this->_snoozeTimer,SIGNAL(timeout()),SLOT(timerOut()));

}

snooze::~snooze()
{
    delete ui;
}

void snooze::SetupClock()
{
    //Set up clock display
    UpdateClock();
    QTimer *CurrentTime=new QTimer(this);
    connect(CurrentTime,SIGNAL(timeout()),this,SLOT(UpdateClock()));
    CurrentTime->start(500);
}


void snooze::UpdateClock()
{
    ui->time->setText(QTime::currentTime().toString("h:mm:ss ap"));
}

void snooze::SnoozeClicked()
{
    this->_snoozeTimer->start(300000);//5 minutes
    this->_curAlarm->Stop();
}

void snooze::DismissClicked()
{
    this->_snoozeTimer->stop();
    this->_curAlarm->Stop();
    this->hide();
    this->~snooze();
}

void snooze::timerOut()
{
    this->_curAlarm->Start(this->_curAlarm->UsingCustomPath);
    this->_curAlarm->canResume=false;
}
