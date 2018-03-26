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

#include "coreaction.h"
#include "ui_coreaction.h"

#include <QDesktopWidget>
#include <QDateTime>
#include <QTimer>
#include <QFile>
#include <QDoubleValidator>


float varA, varB, result;
int z = 0, varC, varD;

coreaction::coreaction(QWidget *parent) : QWidget(parent, Qt::Dialog),
    ui(new Ui::coreaction)
{
    qDebug() << "coreaction opening";
    ui->setupUi(this);

    QScreen *screen = QGuiApplication::primaryScreen();
    QRect  screenGeometry = screen->geometry();
    int x = screenGeometry.width()* .23;
    int y = screenGeometry.height();

    int sw = screenGeometry.width() - x;

    this->setFixedSize(x, y);
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint); //| Qt::ToolTip

    this->move(sw, 0);

    //DEMO
//    QStringList lio;
//    lio.append("/home/shaber/Downloads/");
//    lio.append("/home/shaber/Desktop/");
//    qDebug() << "DEMO FOR GETTING FILESIZE " << getMultipleFileSize(lio);
    //

    ui->calcview->setValidator(new QDoubleValidator(0,99999999,99999999,this));

    ui->bookmarks->setVisible(0);
    ui->dashboard->setVisible(0);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
    timer->start(1000);

    loadsettings();
    tryicon();
    batteryCheck();
    showTime();
    messageEngine("CoreAction Started.\nPlease click icon in System Tray", "Info");
}

coreaction::~coreaction()
{
    qDebug()<<"coreaction closing";
    delete ui;
}

void coreaction::tryicon()  //setup coreaction tryicon
{
    trayIcon=new QSystemTrayIcon(this);
    trayIconMenu=new QMenu(this);
    QAction *QAshow=new QAction("&Show",this);
    QAction *QAabout=new QAction("&About",this);
    QAction *QAquit=new QAction("&Quit",this);
    trayIconMenu->addAction(QAshow);
    trayIconMenu->addAction(QAabout);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(QAquit);
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setIcon(QIcon(":/icons/CoreAction.svg"));
    trayIcon->setToolTip("CoreAction");
    trayIcon->show();
    connect(QAquit, SIGNAL(triggered()),qApp,SLOT(quit()));
    connect(QAshow, SIGNAL(triggered()),this,SLOT(show()));
    connect(QAabout, SIGNAL(triggered()), this, SLOT(on_about_clicked()));
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(ShowWindow(QSystemTrayIcon::ActivationReason)));
}

void coreaction::loadsettings()
{
    SettingsManage sm;
    sm.createDefaultSettings();
}

void coreaction::batteryCheck()
{
    if (sm.getShowBattery()) {
        UPower *u = new UPower(this);
        Battery *b;
        foreach (Battery *bat, *u->batteries()) {
            b = u->batteries()->value(bat->path());
        }
        ui->batteryProg->setValue(b->percentage());

        switch( b->state() ) {
            case Battery::FullyCharged:
                ui->batteryStatus->setText( tr( "Full" ) );
                break;
            case Battery::Discharging:
                ui->batteryStatus->setText( tr( "Discharging" ) );
                break;
            case Battery::Charging:
                ui->batteryStatus->setText( tr( "Charging" ) );
                break;
            default:
                ui->batteryStatus->setText( tr( "No Battery" ) );
                break;
        }
        ui->batteryframe->setVisible(true);
    } else {
        ui->batteryframe->setVisible(false);
    }
}

void coreaction::on_about_clicked() {
    about *ab = new about();
    ab->show();
}

void coreaction::showTime()
{
    QTime time = QTime::currentTime();
    QString text = time.toString("hh:mm");
    if ((time.second() % 2) == 0)
        text[2] = ' ';
    QString d = QDate::currentDate().toString("dddd");
    QString dt = QDate::currentDate().toString("MMMM d");
    ui->day->setText(d + ",");
    ui->date->setText(dt);
}

void coreaction::focusOutEvent(QFocusEvent *event){
    if (event->lostFocus()) {
    }
    if (geometry().contains(QCursor::pos())) {
        setFocus(Qt::PopupFocusReason);
    } else {
        hide();
    }
}

void coreaction::ShowWindow(QSystemTrayIcon::ActivationReason Reason){
    if (Reason == QSystemTrayIcon::DoubleClick || Reason == QSystemTrayIcon::Trigger) {
        if (!this->isVisible()) {
            batteryCheck();
            this->show();
        } else {
            this->hide();
        }
    }
}

void coreaction::on_hide_clicked()
{
    this->hide();
}

//--------------------Calculator----------------------Start---------------------------
void coreaction::on_one_clicked()
{
    ui->calcview->setText(ui->calcview->text() + "1");
}

void coreaction::on_two_clicked()
{
    ui->calcview->setText(ui->calcview->text() + "2");
}

void coreaction::on_three_clicked()
{
    ui->calcview->setText(ui->calcview->text() + "3");
}

void coreaction::on_four_clicked()
{
    ui->calcview->setText(ui->calcview->text() + "4");
}

void coreaction::on_five_clicked()
{
    ui->calcview->setText(ui->calcview->text() + "5");
}

void coreaction::on_six_clicked()
{
    ui->calcview->setText(ui->calcview->text() + "6");
}

void coreaction::on_seven_clicked()
{
    ui->calcview->setText(ui->calcview->text() + "7");
}

void coreaction::on_eight_clicked()
{
    ui->calcview->setText(ui->calcview->text() + "8");
}

void coreaction::on_nine_clicked()
{
    ui->calcview->setText(ui->calcview->text() + "9");
}

void coreaction::on_zero_clicked()
{
    ui->calcview->setText(ui->calcview->text() + "0");
}

void coreaction::on_dot_clicked()
{
    ui->calcview->setText(ui->calcview->text() + ".");
}

void coreaction::on_equal_clicked()
{
    varB = ui->calcview->text().toFloat();
    switch(z)
    {
    case 1 : result = varA + varB ;break;
    case 2 : result = varA - varB ;break;
    case 3 : result = varA * varB ;break;
    case 4 : result = varA / varB ;break;
    case 5 :
        varC = varA;
        varD = varB;
        result = varC % varD ;break;
    }
    QString sss = QString::number(result);
    ui->calcview->setText(sss);
}

void coreaction::on_clear_clicked()
{
    ui->calcview->setText("");
}

void coreaction::on_multiply_clicked()
{
    z = 3;
    varA = ui->calcview->text().toFloat();
    ui->calcview->setText("");
}

void coreaction::on_subtract_clicked()
{
    z = 2;
    varA = ui->calcview->text().toFloat();
    ui->calcview->setText("");
}

void coreaction::on_add_clicked()
{
    z = 1;
    varA = ui->calcview->text().toFloat();
    ui->calcview->setText("");
}

void coreaction::on_division_clicked()
{
    z = 4;
    varA = ui->calcview->text().toFloat();
    ui->calcview->setText("");
}

void coreaction::on_percentage_clicked()
{
    z = 5;
    varA = ui->calcview->text().toFloat();
    ui->calcview->setText("");
}
//--------------------Calculator----------------------End---------------------------


void coreaction::on_corepad_clicked()
{
    CoreBox *cBox = new CoreBox();
    cBox->show();
    cBox->tabEngine(CorePad);
    this->hide();
}

void coreaction::on_screenshot_clicked()
{
    MainWindow *w = new MainWindow();
    w->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    w->show();
    this->hide();
}

void coreaction::on_corepaint_clicked()
{
    CoreBox *cBox = new CoreBox();
    cBox->show();
    cBox->tabEngine(CorePaint);
    this->hide();
}

void coreaction::on_corefm_clicked()
{
    CoreBox *cBox = new CoreBox();
    cBox->show();
    cBox->tabEngine(CoreFM);
    this->hide();
}

void coreaction::on_start_clicked()
{
    CoreBox *cBox = new CoreBox();
    cBox->show();
    this->hide();
}

void coreaction::on_search_clicked()
{
    CoreBox *cBox = new CoreBox();
    cBox->show();
    cBox->tabEngine(Search);
    this->hide();
}
