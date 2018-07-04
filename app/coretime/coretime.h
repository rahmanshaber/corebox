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

#ifndef CORETIME_H
#define CORETIME_H

#include "timer.h"
#include "alarm.h"
#include "fileio.h"
#include "schedulecollection.h"
#include "snooze.h"

#include <QWidget>
#include <QTime>
#include <QDateTime>
#include <QElapsedTimer>
#include <QDialogButtonBox>
#include <QListWidgetItem>
#include <QCheckBox>
#include <QLabel>
#include <QCloseEvent>
#include <QTimeEdit>
#include <QTimer>
#include <QFileDialog>
#include <QDialogButtonBox>
#include <QSlider>
#include <QListWidgetItem>
#include <QCalendarWidget>

#include "../corebox/corebox.h"
#include "../corebox/globalfunctions.h"


namespace Ui {
class coretime;
}

class coretime : public QWidget
{
    Q_OBJECT

public:
    explicit coretime(QWidget *parent = 0);
    ~coretime();

    QString msToTime(quint64 ms);
    quint64 timeToms(QString time);

private:
    Ui::coretime *ui;
    QTimer *forStopWatch;

    QElapsedTimer *stopwatch;
    QTimer *forTimer;

    bool mRunning;
    QDateTime mStartTime;
    qint64 mTotalTime;
    qint64 mSessionTime;

    Timer *TimeKeeper;
    Alarm *CurAlarm;
    ScheduleCollection *_Schedules;

    int _lastDeletedIndex;
    bool _isMilTime;
    bool _prevTimeWasMil;
    bool _supportsTray;
    bool _WarnOnPm;
    void displayTimeMode();

    void SetupClock();
    void UpdateClock();
    void PMWarning();
    void PopulateListWidget();
    void DisablePanelIfNoSelection();
    void UpdateListWidget();

private slots:
    void updateStopwatch();

    void ShowActiveAlarm(int index);
    void SetTime();
    void SetCustomDate();
    void ToggleMon(bool);
    void ToggleTue(bool);
    void ToggleWed(bool);
    void ToggleThur(bool);
    void ToggleFri(bool);
    void ToggleSat(bool);
    void ToggleSun(bool);
    void ToggleCust(bool);
    void OpenDiaglog(bool);
    void TestAlarm();
    void SnoozeMenuCheck();
    void timeCheck();

    void on_startandpause_clicked(bool checked);
    void on_lapandreset_clicked();
    void on_addTimer_clicked();
    void on_startstopTimer_clicked(bool checked);
    void on_resetTimer_clicked();
    void counting();

    void on_listWidget_currentRowChanged(int currentRow);
    void on_editalarm_clicked();
    void on_removealarm_clicked();
    void on_addalarm_clicked();

    void pageClick(QPushButton *btn, int i, QString title);
    void on_alarm_clicked();
    void on_stopwatch_clicked();
    void on_timer_clicked();

public slots:
    void start();
    void resume();
    void pause();
    void stop();

};

#endif // CORETIME_H
