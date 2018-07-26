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
#include "corebox/globalfunctions.h"

coreaction::coreaction(QWidget *parent) : QWidget(parent, Qt::Dialog),ui(new Ui::coreaction),
    timer(new QTimer(this))
{
    ui->setupUi(this);

    // set stylesheet from style.qrc
    setStyleSheet(getStylesheetFileContent(":/appStyle/style/CoreAction.qss"));

    widget();
    loadsettings();
    widgetList();
    tryicon();

    addDropShadow(ui->appsW, 60, 50);
}

coreaction::~coreaction()
{
    delete ui;
}

void coreaction::widget()  //setup coreaction widget
{
    int x = screensize().width()* .23;
    int y = screensize().height();
    int sw = screensize().width() - x;

    ui->widgetsL->setFixedWidth(x);

    this->setFixedSize(x+6, y);
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint); //| Qt::ToolTip
    this->move(sw, 0);
    QTimer::singleShot(8000, this, SLOT(hide()));
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
    trayIcon->setIcon(QIcon(":/app/icons/app-icons/CoreAction.svg"));
    trayIcon->setToolTip("CoreAction");
    trayIcon->show();

    connect(QAquit, SIGNAL(triggered()),this,SLOT(close()));
    connect(QAshow, SIGNAL(triggered()),this,SLOT(actionshow()));
    connect(QAabout, &QAction::triggered, this, &coreaction::coreBoxAbout);
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(ShowWindow(QSystemTrayIcon::ActivationReason)));
}

void coreaction::widgetList() //setup all enabled widgets for settings
{
    ui->timeW->hide();
    checkWTime();
    checkWCalendar();
    checkWSystem();
    checkWNetwork();
    checkWCalculator();
    checkWNotes();
    checkWBattery();
}

bool coreaction::checkWTime()
{
    if(sm.getShowTime() == 1){
        ui->timeW->setVisible(1);
        QTimer *timer = new QTimer(this);
        showTime();
        connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
        timer->start(1000);
        return true;
    } else {
        ui->timeW->setVisible(0);
    }
    return false;
}

bool coreaction::checkWCalendar()
{
    wCalender *wcd = ui->widgetsL->findChild<wCalender*>("wCalender");
    if(sm.getShowCalander() == 1){
        if (wcd)
            return true;

        ui->verticalLayout_6->insertWidget(0, new wCalender(ui->widgetsL));
        return true;
    } else {
        if (wcd)
            wcd->deleteLater();
    }
    return false;
}

bool coreaction::checkWSystem()
{
    wSystem *wsys = ui->widgetsL->findChild<wSystem*>("wSystem");
    if(sm.getShowSystem() == 1){
        if (wsys)
            return true;

        ui->verticalLayout_6->insertWidget(1, new wSystem(ui->widgetsL));
        return true;
    } else {
        if (wsys)
            wsys->deleteLater();
    }
    return false;
}

bool coreaction::checkWNetwork()
{
    wNetwork *wnw = ui->widgetsL->findChild<wNetwork*>("wNetwork");
    if(sm.getShowNetwork() == 1){
        if (wnw)
            return true;

        ui->verticalLayout_6->insertWidget(2, new wNetwork(ui->widgetsL));
        return true;
    } else {
        if (wnw)
            wnw->deleteLater();
    }
    return false;
}

bool coreaction::checkWCalculator()
{
    wCalculator *wcl = ui->widgetsL->findChild<wCalculator*>("wCalculator");
    if(sm.getShowCalculator() == 1){
        if (wcl)
            return true;

        ui->verticalLayout_6->insertWidget(3, new wCalculator(ui->widgetsL));
        return true;
    } else {
        if (wcl)
            wcl->deleteLater();
    }
    return false;
}

bool coreaction::checkWNotes()
{
    wNotes *wno = ui->widgetsL->findChild<wNotes*>("wNotes");
    if(sm.getShowNote() == 1){
        if (wno)
            return true;

        ui->verticalLayout_6->insertWidget(4, new wNotes(ui->widgetsL));
        return true;
    } else {
        if (wno)
            wno->deleteLater();
    }
    return false;
}

bool coreaction::checkWBattery()
{
    wBattery *wbt = ui->widgetsL->findChild<wBattery*>("wBattery");
    if(sm.getShowBattery() == 1){
        if (wbt)
            return true;

        ui->verticalLayout_6->insertWidget(5, new wBattery(ui->widgetsL));
        return true;
    } else {
        if (wbt)
            wbt->deleteLater();
    }
    return false;
}

void coreaction::loadsettings() //load setting ini
{
    SettingsManage sm;
    sm.createDefaultSettings();
}

void coreaction::actionshow()
{
    widgetList();
    this->show();
}

void coreaction::coreBoxAbout()
{
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

void coreaction::focusOutEvent(QFocusEvent *event)
{
    Q_UNUSED(event);
    if (event->lostFocus()) {
    }
    if (geometry().contains(QCursor::pos())) {
        setFocus(Qt::PopupFocusReason);
    } else {
        hide();
    }
}

void coreaction::ShowWindow(QSystemTrayIcon::ActivationReason Reason)
{
    if (Reason == QSystemTrayIcon::DoubleClick || Reason == QSystemTrayIcon::Trigger) {
        if (!this->isVisible()) {
            //batteryCheck();
            this->show();
            QTimer::singleShot(8000, this, SLOT(hide()));
        } else {
            this->hide();
        }
    }
}

void coreaction::on_corepad_clicked()
{
    CoreBox *cBox = new CoreBox();
    cBox->show();
    cBox->tabEngine(CorePad);
    this->hide();
}

void coreaction::on_screenshot_clicked()
{
    coreshotdialog *w = new coreshotdialog();
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
    cBox->tabEngine(StartView);
    this->hide();
}

void coreaction::on_search_clicked()
{
    CoreBox *cBox = new CoreBox();
    cBox->show();
    cBox->tabEngine(Search);
    this->hide();
}

void coreaction::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    QTimer::singleShot(100, qApp, SLOT(quit()));
}
