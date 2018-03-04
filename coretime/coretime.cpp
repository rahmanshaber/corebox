#include "coretime.h"
#include "ui_coretime.h"
#include "timer.h"
#include "alarm.h"
#include "fileio.h"
#include "schedulecollection.h"
#include "settingdialog.h"
#include "snooze.h"
#include "corebox.h"
#include "globalfunctions.h"

#include <QCheckBox>
#include <QLabel>
#include <QCloseEvent>
#include <QTimeEdit>
#include <QTimer>
#include <QFileDialog>
#include <QDialogButtonBox>
#include <QSlider>
#include <QSystemTrayIcon>
#include <QListWidgetItem>
#include <QCalendarWidget>


coretime::coretime(QWidget *parent) :QWidget(parent),ui(new Ui::coretime)
{
    qDebug() << "coretime opening";
    ui->setupUi(this);

    //Create / load Schedule
    _Schedules=new ScheduleCollection(this);
    _Schedules->LoadSchedules();
    PopulateListWidget();
    DisablePanelIfNoSelection();

    _isMilTime=FileIO::isMilTime();
    _WarnOnPm=FileIO::LoadWarnOnPm();
    _prevTimeWasMil=_isMilTime;
    displayTimeMode();

    //Call Time keeper
    TimeKeeper=new Timer(this,_Schedules);
    CurAlarm=new Alarm(this);
    TimeKeeper->StartTimer(CurAlarm);

    //Set Volume
    int Volume = FileIO::LoadVolume();
    ui->VolumeSlider->setValue(Volume<=0? 50:Volume);
    CurAlarm->SetVolume(ui->VolumeSlider->value());
    ui->listAlmBtn->button(QDialogButtonBox::Ok)->setText("&Add");
    ui->listAlmBtn->button(QDialogButtonBox::Cancel)->setText("&Remove");

    ui->txtSoundPath->setText("");
    ui->CustEdit->setDate(QDate::currentDate());
    ui->timerDuration->setVisible(0);
    SetupClock();


    //set up ui slots
    connect(ui->timeEdit,SIGNAL(editingFinished()),this,SLOT(SetTime()));
    connect(ui->listAlmBtn,SIGNAL(clicked(QAbstractButton*)),this,SLOT(AddRemoveAlarm(QAbstractButton*)));
    connect(ui->listWidget,SIGNAL(currentRowChanged(int)),this,SLOT(ShowActiveAlarm(int)));
    connect(ui->chkMon,SIGNAL(clicked(bool)),this,SLOT(ToggleMon(bool)));
    connect(ui->chkTues,SIGNAL(clicked(bool)),this,SLOT(ToggleTue(bool)));
    connect(ui->chkWed,SIGNAL(clicked(bool)),this,SLOT(ToggleWed(bool)));
    connect(ui->chkThurs,SIGNAL(clicked(bool)),this,SLOT(ToggleThur(bool)));
    connect(ui->chkFri,SIGNAL(clicked(bool)),this,SLOT(ToggleFri(bool)));
    connect(ui->chkSat,SIGNAL(clicked(bool)),this,SLOT(ToggleSat(bool)));
    connect(ui->chkSun,SIGNAL(clicked(bool)),this,SLOT(ToggleSun(bool)));
    connect(ui->chkCustom,SIGNAL(clicked(bool)),this,SLOT(ToggleCust(bool)));
    connect(ui->chkSounds,SIGNAL(clicked(bool)),this,SLOT(OpenDiaglog(bool)));
    connect(ui->TestBtn,SIGNAL(clicked()),this,SLOT(TestAlarm()));
    connect(ui->VolumeSlider,SIGNAL(valueChanged(int)),CurAlarm,SLOT(SetVolume(int)));

    connect(ui->calendarWidget,SIGNAL(clicked(QDate)),this,SLOT(SetCustomDate()));

    mRunning = false;
    mTotalTime = 0;
    ui->lapText->clear();
    ui->lapTime->setEnabled(0);
    ui->lapTime->setEnabled(0);
    forStopWatch = new QTimer();
    connect(forStopWatch, SIGNAL(timeout()), this, SLOT(counting()));

}

coretime::~coretime()
{
    delete ui;
}

void coretime::SetupClock()
{
    //Set up clock display
    QTimer *CurrentTime=new QTimer(this);
    connect(CurrentTime,SIGNAL(timeout()),this,SLOT(timeCheck()));
    CurrentTime->start(500);
}

void coretime::ShowWindow(QSystemTrayIcon::ActivationReason Reason)
{
    if(Reason==QSystemTrayIcon::DoubleClick || Reason==QSystemTrayIcon::Trigger)
    {
        ShowWindow();
    }
}

void coretime::ShowWindow()
{
    if(this->CurAlarm->isPlaying())
    {
        this->CurAlarm->Stop();
    }
    ui->TestBtn->setText("Test");
    this->show();
}

void coretime::SetTime()
{
    //If there is no selected row. Enter was probably hit, the time registered, folowed by
    //on change trigger, execpt the changes have already been logged
    if(ui->listWidget->currentIndex().column()!=-1)
    {
        if(ui->timeEdit->time().hour()>12 && !_isMilTime && _WarnOnPm)
        {
            PMWarning();
        }
        Schedule *Active=this->_Schedules->GetSchedule(ui->listWidget->currentRow());
        Active->SetTime(ui->timeEdit->time());
        UpdateListWidget();
    }
}

void coretime::SetCustomDate()
{
    if(ui->listWidget->currentIndex().column()!=-1)
    {
        //Update date on the display
        ui->CustEdit->setDate(ui->calendarWidget->selectedDate());
        Schedule *Active=this->_Schedules->GetSchedule(ui->listWidget->currentRow());
        Active->SetTime(ui->timeEdit->time());
        QDate CustomDate=ui->calendarWidget->selectedDate();
        Active->SetCust(CustomDate);
        if(Active->isCustomEnabled())
            UpdateListWidget();
    }
}

void coretime::ToggleMon(bool isEnabled)
{
    Schedule *Active=this->_Schedules->GetSchedule(ui->listWidget->currentRow());
    Active->setIsMonEnabled(isEnabled);
    UpdateListWidget();
}

void coretime::ToggleTue(bool isEnabled)
{
    Schedule *Active=this->_Schedules->GetSchedule(ui->listWidget->currentRow());
    Active->setIsTueEnabled(isEnabled);
    UpdateListWidget();
}
void coretime::ToggleWed(bool isEnabled)
{
    Schedule *Active=this->_Schedules->GetSchedule(ui->listWidget->currentRow());
    Active->setIsWedEnabled(isEnabled);
    UpdateListWidget();
}
void coretime::ToggleThur(bool isEnabled)
{
    Schedule *Active=this->_Schedules->GetSchedule(ui->listWidget->currentRow());
    Active->setIsThurEnabled(isEnabled);
    UpdateListWidget();
}
void coretime::ToggleFri(bool isEnabled)
{
    Schedule *Active=this->_Schedules->GetSchedule(ui->listWidget->currentRow());
    Active->setIsFriEnabled(isEnabled);
    UpdateListWidget();
}
void coretime::ToggleSat(bool isEnabled)
{
    Schedule *Active=this->_Schedules->GetSchedule(ui->listWidget->currentRow());
    Active->setIsSatEnabled(isEnabled);
    UpdateListWidget();
}
void coretime::ToggleSun(bool isEnabled)
{
    Schedule *Active=this->_Schedules->GetSchedule(ui->listWidget->currentRow());
    Active->setIsSunEnabled(isEnabled);
    UpdateListWidget();
}


void coretime::ToggleCust(bool isEnabled)
{
    Schedule *Active=this->_Schedules->GetSchedule(ui->listWidget->currentRow());
    Active->SetCustEnabled(isEnabled);
    UpdateListWidget();
}

void coretime::Quit()
{
    this->_Schedules->Save();
    FileIO::DelExtracted();
    FileIO::SaveVolume(ui->VolumeSlider->value());
    qApp->quit();
}

void coretime::AddRemoveAlarm(QAbstractButton *button)
{
    if(button->text()=="&Add")
    {
        Schedule *scheToAdd=new Schedule(this);
        this->_Schedules->AddSchedule(scheToAdd);
        ui->listWidget->addItem(scheToAdd->Name());
        ui->listWidget->setCurrentRow(ui->listWidget->count()-1);
    }
    else if(button->text()=="&Remove")
    {
        this->_lastDeletedIndex=ui->listWidget->currentRow();
        this->_Schedules->removeScheduleByIndex(ui->listWidget->currentRow());
        PopulateListWidget();
    }
}

void coretime::ShowActiveAlarm(int index)
{
    DisablePanelIfNoSelection();
    if(index==-1)
    {
        //in the middle of a list clear. Running further will cause seg fault
        return;
    }
    Schedule *active=this->_Schedules->GetSchedule(index);
    ui->timeEdit->setTime(active->GetTime());

    ui->chkCustom->setChecked(active->GetCustomEnabled());
    ui->CustEdit->setDate(active->GetCustomDate());
    ui->chkSounds->setChecked(active->GetCustomSoundEnabled());
    ui->txtSoundPath->setText(active->GetCustomSound());

    ui->chkMon->setChecked(active->isMonEnabled());
    ui->chkTues->setChecked(active->isTueEnabled());
    ui->chkWed->setChecked(active->isWedEnabled());
    ui->chkThurs->setChecked(active->isThurEnabled());
    ui->chkFri->setChecked(active->isFriEnabled());
    ui->chkSat->setChecked(active->isSatEnabled());
    ui->chkSun->setChecked(active->isSunEnabled());
    ui->calendarWidget->setSelectedDate(active->GetCustomDate());
}

void coretime::timeCheck()
{
    UpdateClock();
    SnoozeMenuCheck();
    if(_isMilTime!=_prevTimeWasMil)
    {
        _prevTimeWasMil=_isMilTime;
        displayTimeMode();
    }
}

void coretime::UpdateClock()
{
    if(_isMilTime)
    {
        ui->Clock->setText(QTime::currentTime().toString("H:mm:ss"));
    }else{
        ui->Clock->setText(QTime::currentTime().toString("h:mm:ss ap"));
    }
}


void coretime::OpenDiaglog(bool isChecked)
{
    Schedule *Active=this->_Schedules->GetSchedule(ui->listWidget->currentRow());
    Active->SetCustomSoundEnabled(isChecked);
    if(isChecked)
    {
        QString AlarmLocation = QFileDialog::getOpenFileName(this,"Select Alarm File",QDir::homePath());
        Active->SetCustomSound(AlarmLocation);
        ui->txtSoundPath->setText(AlarmLocation);
    }else{
        ui->txtSoundPath->setText("");
    }
}

void coretime::TestAlarm()
{
    if(ui->TestBtn->text()=="Test" || ui->TestBtn->text()=="&Test")
    {
        if(ui->chkSounds->isChecked())
        {
            this->CurAlarm->SetCustomPath(ui->txtSoundPath->text());
            this->CurAlarm->Start(true);
        }else{
            this->CurAlarm->Start(false);
        }
        ui->TestBtn->setText("Stop");
    }else{
        this->CurAlarm->Stop();
        ui->TestBtn->setText("Test");
    }
}

void coretime::SnoozeMenuCheck()
{
    if(this->CurAlarm->isPlaying() && this->CurAlarm->canResume && (ui->TestBtn->text()=="Test" || ui->TestBtn->text()=="&Test"))
    {
        //Create Snooze Menu object
        snooze *snMenu=new snooze(this,CurAlarm);
        snMenu->show();
        if(this->_supportsTray)
        {
            this->hide();
        }
        this->CurAlarm->canResume=false;
    }
}


void coretime::PMWarning()
{
    QString mess = "Your selected time is set for PM."
                   " If you intended to wake in the morning, now is your chance to fix it";
    messageEngine(mess,"Warning");
}


void coretime::ShowSettings()
{
    SettingDialog *settingsPage=new SettingDialog(this,&_isMilTime, &_WarnOnPm);
    settingsPage->show();
}

void coretime::displayTimeMode()
{
    if(_isMilTime)
    {
        ui->timeEdit->setDisplayFormat("H:mm:ss");
        ui->CustEdit->setDisplayFormat("d MMM yyyy");
    }else{
        ui->timeEdit->setDisplayFormat("h:mm:ss ap");
        ui->CustEdit->setDisplayFormat("d MMM yyyy");

    }
}

void coretime::PopulateListWidget()
{
    ui->listWidget->clear();
    Schedule *sche;
    foreach(sche,this->_Schedules->GetScheduleList())
    {
        ui->listWidget->addItem(sche->Name());
    }
    ui->listWidget->setCurrentRow(this->_lastDeletedIndex);
}

void coretime::DisablePanelIfNoSelection()
{
    if(ui->listWidget->currentRow()==-1)
    {
        ui->chkCustom->setEnabled(false);
        ui->chkFri->setEnabled(false);
        ui->chkMon->setEnabled(false);
        ui->chkSat->setEnabled(false);
        ui->chkSounds->setEnabled(false);
        ui->chkSun->setEnabled(false);
        ui->chkThurs->setEnabled(false);
        ui->chkTues->setEnabled(false);
        ui->chkWed->setEnabled(false);
        ui->CustEdit->setEnabled(false);
        ui->timeEdit->setEnabled(false);

        ui->chkCustom->setChecked(false);
        ui->chkFri->setChecked(false);
        ui->chkMon->setChecked(false);
        ui->chkSat->setChecked(false);
        ui->chkSounds->setChecked(false);
        ui->chkSun->setChecked(false);
        ui->chkThurs->setChecked(false);
        ui->chkTues->setChecked(false);
        ui->chkWed->setChecked(false);
        ui->txtSoundPath->setText("");

    }
    else
    {
        ui->chkCustom->setEnabled(true);
        ui->chkFri->setEnabled(true);
        ui->chkMon->setEnabled(true);
        ui->chkSat->setEnabled(true);
        ui->chkSounds->setEnabled(true);
        ui->chkSun->setEnabled(true);
        ui->chkThurs->setEnabled(true);
        ui->chkTues->setEnabled(true);
        ui->chkWed->setEnabled(true);
        ui->CustEdit->setEnabled(true);
        ui->timeEdit->setEnabled(true);
    }
}

void coretime::UpdateListWidget()
{
    int index=ui->listWidget->currentRow();
    PopulateListWidget();
    ui->listWidget->setCurrentRow(index);
}

void coretime::counting() {
    ui->sw->setText(msToTime(timeToms(ui->sw->text()) + 1));
}

QString coretime::msToTime(quint64 ms) {
    unsigned int h = ms / 1000 /60 /60;
    unsigned int m = (ms / 1000 /60) - (h * 60);
    unsigned int s = (ms / 1000) - ((m + (h * 60)) * 60);
    unsigned int mis = ms - (s + ((m + (h * 60)) * 60)) * 1000;
    const QString diff = QString("%1:%2:%3.%4")
            .arg(h, 2, 10, QChar('0'))
            .arg(m, 2, 10, QChar('0'))
            .arg(s, 2, 10, QChar('0'))
            .arg(mis, 3, 10, QChar('0'));
    return diff;
}

quint64 coretime::timeToms(QString time) {
    quint64 total;
    QTime dt = QDateTime::fromString(time, "hh:mm:ss.zzz").time();
    total = ((((dt.hour()) * 60) * 60) * 1000) + ((dt.minute() * 60) * 1000) + (dt.second() * 1000) + dt.msec();
    return total;
}

void coretime::start() {
    forStopWatch->start(1);
}

void coretime::resume() {
    forStopWatch->start();;
}

void coretime::pause() {
    int remain = forStopWatch->remainingTime();
    forStopWatch->stop();
    forStopWatch->setInterval(remain);
}

void coretime::stop() {
    forStopWatch->stop();
}

void coretime::updateStopwatch() {
    if (!(ui->timerText->text() == "00:00:00")) {
        QTime time = QDateTime::fromString(ui->timerText->text(), "hh:mm:ss").time();
        qint64 total = ((time.hour() * 60) * 60) + (time.minute() * 60) + (time.second() - 1);
        int h = total / 60 / 60;
        int m = (total / 60) - (h * 60);
        int s = total - ((m + (h * 60)) * 60);
        time.setHMS(h, m, s);
        ui->timerText->setText(time.toString("hh:mm:ss"));
    }
}

void coretime::on_startandpause_clicked(bool checked)
{
    if (checked) {
        ui->startandpause->setText("Pause");
        ui->lapandreset->setText("Lap");
        if (ui->startandpause->text() == "Resume") {
            resume();
        } else {
            start();
        }
    } else {
        ui->startandpause->setText("Resume");
        ui->lapandreset->setText("Reset");
        pause();
    }
}


void coretime::on_lapandreset_clicked()
{
    if (ui->lapandreset->text() == "Reset") {
        stop();
        ui->startstopTimer->setText("Start");
        ui->sw->setText("00:00:00.000");
        ui->lapText->clear();
        ui->lapTime->clear();
    } else {
        ui->lapText->appendPlainText("#" + QString::number(ui->lapText->document()->lineCount()) + "   " + "" + "   " + ui->sw->text());
        if (ui->lapTime->count() == 0) {
            ui->lapTime->addItem("#" + QString::number(ui->lapTime->count() + 1) + "   "
                                 + ui->sw->text() + "   "
                                 + ui->sw->text());
        } else {
            ui->lapTime->addItem("#" + QString::number(ui->lapTime->count() + 1) + "   "
                                 + QString::number(mSessionTime) + " " + QString::number(
                                     QDateTime::fromString(ui->lapTime->item(ui->lapTime->count() - 1)->text().split("   ").at(2), "hh:mm:ss.zzz").time().second()) + "   "
                                 + ui->sw->text());
        }
    }
}

void coretime::on_addTimer_clicked()
{
    ui->timerText->setText(ui->setTimer->text());
    ui->timerDuration->setVisible(true);
    ui->startstopTimer->setText("Pause");
    ui->startstopTimer->setChecked(true);
    ui->timerSet->setVisible(false);
    forTimer = new QTimer();
    forTimer->start(1000);
    connect(forTimer, SIGNAL(timeout()), this, SLOT(updateStopwatch()));
    ui->timerDuration->setVisible(1);
}

void coretime::on_startstopTimer_clicked(bool checked)
{
    if (checked) {
        ui->startstopTimer->setText("Pause");

    } else {
        ui->startstopTimer->setText("Start");
        forTimer->disconnect(0,0,0,0);
    }
}

void coretime::on_resetTimer_clicked()
{
    ui->timerText->setText("00:00:00");
    ui->startstopTimer->setChecked(false);
    ui->startstopTimer->setText("Start");
    disconnect(forTimer, 0, 0, 0);
    ui->timerSet->setVisible(true);
}

void coretime::on_alarm_clicked(bool checked)
{
    if(checked){
        ui->selectedsection->setText("Alarm");
        ui->pages->setCurrentIndex(0);
        ui->stopwatch->setChecked(false);
        ui->timer->setChecked(false);
    }
}

void coretime::on_stopwatch_clicked(bool checked)
{
    if(checked){
        ui->selectedsection->setText("Stopwatch");
        ui->pages->setCurrentIndex(1);
        ui->alarm->setChecked(false);
        ui->timer->setChecked(false);
    }
}

void coretime::on_timer_clicked(bool checked)
{
    if(checked){
        ui->selectedsection->setText("Timer");
        ui->pages->setCurrentIndex(2);
        ui->alarm->setChecked(false);
        ui->stopwatch->setChecked(false);
    }
}
