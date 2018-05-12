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
    ui(new Ui::coreaction),
    timer(new QTimer(this)),
    im(InfoManager::ins())
{
    qDebug() << "coreaction opening";
    ui->setupUi(this);

    int x = screensize().width()* .23;
    int y = screensize().height();

    int sw = screensize().width() - x;

    ui->widgetsL->setFixedWidth(x);

    this->setFixedSize(x+6, y);
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint); //| Qt::ToolTip
    this->move(sw, 0);
    QTimer::singleShot(8000, this, SLOT(hide()));

    loadsettings();
    widgetList();
    tryicon();
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

    connect(QAquit, SIGNAL(triggered()),this,SLOT(close()));
    connect(QAshow, SIGNAL(triggered()),this,SLOT(show()));
    connect(QAabout, SIGNAL(triggered()), this, SLOT(on_about_clicked()));
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(ShowWindow(QSystemTrayIcon::ActivationReason)));
}

void coreaction::widgetList(){

    if(sm.getShowTime() == 1){
        ui->timeW->setVisible(1);
        QTimer *timer = new QTimer(this);
        showTime();
        connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
        timer->start(1000);
    }else {
        ui->timeW->setVisible(0);
    }

    if(sm.getShowBattery() == 1){
        ui->batteryW->setVisible(1);
        QTimer *timer = new QTimer(this);
        batteryCheck();
        connect(timer, SIGNAL(timeout()), this, SLOT(batteryCheck()));
        timer->start(5000);
    } else {
        ui->batteryW->setVisible(0);
    }

    if(sm.getShowSystem() == 1){
        ui->sysW->setVisible(1);
        sysWsetup();
        connect(timer, &QTimer::timeout, this, &coreaction::sysWsetup);
        timer->start(1 * 1000);
    } else {
        ui->sysW->setVisible(0);
    }

    if(sm.getShowNetwork() == 1){
        ui->networkW->setVisible(1);
        networkWsetup();
        connect(timer, &QTimer::timeout, this, &coreaction::networkWsetup);
        timer->start(1 * 1000);
    }else {
        ui->networkW->setVisible(0);
    }

    if(sm.getShowCalander() == 1){
        ui->calandarW->setVisible(1);
    }else {
        ui->calandarW->setVisible(0);
    }

    if(sm.getShowCalculator() == 1){
        ui->calculatorW->setVisible(1);
        ui->calcview->setValidator(new QDoubleValidator(0,99999999,99999999,this));
    }else {
        ui->calculatorW->setVisible(0);
    }

    if(sm.getShowNote() == 1){
        ui->notesW->setVisible(1);
        collectNotes();
    }else {
        ui->notesW->setVisible(0);
    }
}

void coreaction::loadsettings()
{
    SettingsManage sm;
    sm.createDefaultSettings();
}

void coreaction::batteryCheck()
{
    UPower *u = new UPower(this);

    Battery *b = nullptr;
    foreach (Battery *bat, *u->batteries()) {
        b = u->batteries()->value(bat->path());
    }
    if (!b)
        return;

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
    ui->day->setText(d + "," + dt);
}

void coreaction::focusOutEvent(QFocusEvent *event){
    Q_UNUSED(event);
    if (event->lostFocus()) {
//        qDebug()<<"lost";
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
            QTimer::singleShot(8000, this, SLOT(hide()));
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

void coreaction::closeEvent(QCloseEvent *event) {

    Q_UNUSED(event);
    sm.cSetting->beginGroup("Notes");
    sm.cSetting->setValue("Note1", ui->plainTextEdit->toPlainText());
    sm.cSetting->setValue("Note2", ui->plainTextEdit_2->toPlainText());
    sm.cSetting->endGroup();

    QTimer::singleShot(100, qApp, SLOT(quit()));
}

void coreaction::collectNotes() {

    sm.cSetting->beginGroup("Notes");
    ui->plainTextEdit->setPlainText(sm.cSetting->value("Note1").toString());
    ui->plainTextEdit_2->setPlainText(sm.cSetting->value("Note2").toString());
    sm.cSetting->endGroup();
}

void coreaction::sysWsetup()
{
    //set cpu bar value
    int cpuPercent = im->getCpuPercents().at(0);
    ui->cpubar->setValue(cpuPercent);

    //set ram bar value
    im->updateMemoryInfo();
    int memPercent = 0;

    if (im->getMemTotal())
        memPercent = ((double)im->getMemUsed() / (double)im->getMemTotal()) * 100.0;

    ui->rambar->setValue(memPercent);

    //set drive bar value
    int drivePercent = ((double)QStorageInfo("/").bytesTotal() - QStorageInfo("/").bytesFree() / (double)QStorageInfo("/").bytesTotal()) * 100.0;
    ui->drivebar->setValue(drivePercent);
}

void coreaction::networkWsetup()
{
    static quint64 l_RXbytes = im->getRXbytes();
    static quint64 l_TXbytes = im->getTXbytes();
    static quint64 max_RXbytes = 1L << 20; // 1 MEBI
    static quint64 max_TXbytes = 1L << 20; // 1 MEBI

    quint64 RXbytes = im->getRXbytes();
    quint64 TXbytes = im->getTXbytes();

    quint64 d_RXbytes = (RXbytes - l_RXbytes);
    quint64 d_TXbytes = (TXbytes - l_TXbytes);

    QString downText = formatSize(d_RXbytes);
    QString upText = formatSize(d_TXbytes);

    ui->dspeed->setText(downText);
    ui->uspeed->setText(upText);

    max_RXbytes = qMax(max_RXbytes, d_RXbytes);
    max_TXbytes = qMax(max_TXbytes, d_TXbytes);

    l_RXbytes = RXbytes;
    l_TXbytes = TXbytes;
}
