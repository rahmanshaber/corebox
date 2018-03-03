#ifndef CORETIME_H
#define CORETIME_H

#include "timer.h"

#include <QDebug>
#include <QWidget>
#include <QTime>
#include <QDateTime>
#include <QElapsedTimer>
#include <QSystemTrayIcon>
#include <QDialogButtonBox>
#include <QListWidgetItem>


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

    void ShowWindow();
    void ShowActiveAlarm(int index);
    void ShowWindow(QSystemTrayIcon::ActivationReason);
    void AddRemoveAlarm(QAbstractButton*);
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
    void Quit();
    void OpenDiaglog(bool);
    void TestAlarm();
    void SnoozeMenuCheck();
    void timeCheck();
    void ShowSettings();

    void on_startandpause_clicked(bool checked);
    void on_lapandreset_clicked();
    void on_addTimer_clicked();
    void on_startstopTimer_clicked(bool checked);
    void on_resetTimer_clicked();
    void counting();

    void on_alarm_clicked(bool checked);
    void on_stopwatch_clicked(bool checked);
    void on_timer_clicked(bool checked);

public slots:
    void start();
    void resume();
    void pause();
    void stop();

};

#endif // CORETIME_H
