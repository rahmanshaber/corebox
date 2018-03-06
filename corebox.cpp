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

#include "corebox.h"
#include "ui_corebox.h"

#include <QDateTime>
#include <QSettings>
#include <QSystemTrayIcon>
#include <QPoint>

#include "start.h"
#include "bookmarks.h"
#include "about.h"
#include "bookmarkmanage.h"
#include "settings.h"
#include "search/search.h"
#include "dashboard/dashboard.h"
#include "help.h"
#include "corepad/corepad.h"
#include "corefm/corefm.h"
#include "corepaint/corepaint.h"
#include "coreimage/coreimage.h"
#include "coreplayer/coreplayer.h"
#include "coreaction/coreaction.h"
#include "coretime/coretime.h"

CoreBox::CoreBox(QWidget *parent) :QMainWindow(parent),ui(new Ui::CoreBox)
{
    qDebug() << "CoreBox opening";
    ui->setupUi(this);

    QDesktopWidget dw;
    int x =dw.width()*.8;
    int y = dw.height()*.8;

    ui->windows->installEventFilter(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint );//| Qt::SubWindow
    ui->restoreButton->setVisible(false);
    ui->windows->setCornerWidget(ui->winbutn, Qt::BottomRightCorner);

    if (sm.getBoxIsMaximize()) {
        on_maximizeButton_clicked();
    }else{
        this->resize(x,y);
    }

    BookmarkManage bk;
    bk.checkBook();
    on_start_clicked();
}

CoreBox::~CoreBox()
{
    qDebug()<<"corebox closing";
    delete ui;
}

//===========================WindowBar========Start===============================================================

void CoreBox::tabEngine(int i, QString arg) {

    int n = ui->windows->count();
    if (i == 0) {
        corefm *cFM = new corefm();
        if (!arg.isEmpty()) {cFM->goTo(arg); }
        ui->windows->insertTab(n, cFM, QIcon(":/icons/CoreFM.svg"), "CoreFM");
        ui->windows->setCurrentIndex(n);
    } else if (i == 1) {
        coreimage *cIMG = new coreimage();
        cIMG->loadFile(arg);
        ui->windows->insertTab(n, cIMG, QIcon(":/icons/CoreImage.svg"), "Core Image");
        ui->windows->setCurrentIndex(n);
    } else if (i == 2) {
        corepad *cPAD = new corepad();
        if (arg.isEmpty()) {cPAD->openText(arg);}
        ui->windows->insertTab(n, cPAD, QIcon(":/icons/CorePad.svg"), "Core Pad");
        ui->windows->setCurrentIndex(n);
    } else if (i == 3) {
        corepaint *cPAINT = new corepaint();
        cPAINT->initializeNewTab(true, arg);
        ui->windows->insertTab(n, cPAINT, QIcon(":/icons/CorePaint.svg"), "Core Paint");
        ui->windows->setCurrentIndex(n);
    } else if (i == 4) {
        coreplayer *cPLAYER = new coreplayer();
        if (!arg.isEmpty()) { cPLAYER->openPlayer(arg); }
        ui->windows->insertTab(n, cPLAYER, QIcon(":/icons/CorePlayer.svg"), "Core Player");
        ui->windows->setCurrentIndex(n);
    } else if (i == 5) {
        ui->windows->insertTab(n, new dashboard(), QIcon(":/icons/DashBoard.svg"), "DashBoard");
        ui->windows->setCurrentIndex(n);
    } else if (i == 6) {
        ui->windows->insertTab(n, new bookmarks(), QIcon(":/icons/Bookmarks.svg"), "Bookmarks");
        ui->windows->setCurrentIndex(n);
    } else if (i == 7) {
        ui->windows->insertTab(n, new about(), QIcon(":/icons/About.svg"), "About");
        ui->windows->setCurrentIndex(n);
    } else if (i == 8) {
        ui->windows->insertTab(n, new Start(), QIcon(":/icons/Start.svg"), "Start");
        ui->windows->setCurrentIndex(n);
    } else if (i == 9) {
        ui->windows->insertTab(n, new help(), QIcon(":/icons/Help.svg"), "Help");
        ui->windows->setCurrentIndex(n);
    } else if (i == 10) {
        ui->windows->insertTab(n, new settings(), QIcon(":/icons/Settings.svg"), "Settings");
        ui->windows->setCurrentIndex(n);
    } else if (i == 11) {
        search *ser = new search();
        ser->setPath(arg);
        ui->windows->insertTab(n, ser, QIcon(":/icons/Search.svg"), "Search");
        ui->windows->setCurrentIndex(n);
    } else if (i == 12) {
        ui->windows->insertTab(n, new coretime, QIcon(":/icons/CoreTimesvg"), "Core Time");
        ui->windows->setCurrentIndex(n);
    } else if (i == 99) {
        CoreBox *cBox = new CoreBox();
        cBox->show();
    }
}

void CoreBox::on_windows_tabCloseRequested(int index)
{
    QString appName = ui->windows->tabBar()->tabText(index);

    if (appName == "Bookmarks") {
        bookmarks *cbook = ui->windows->findChild<bookmarks*>("bookmarks");
        if (cbook->close()){
            cbook->deleteLater();
            ui->windows->removeTab(index);
        }
    } else if (appName == "About") {
        about *cabout = ui->windows->findChild<about*>("about");
        if (cabout->close()){
            cabout->deleteLater();
            ui->windows->removeTab(index);
        }
    } else if (appName == "DashBoard") {
        dashboard *cdash = ui->windows->findChild<dashboard*>("dashboard");
        if (cdash->close()){
            cdash->deleteLater();
            ui->windows->removeTab(index);
        }
    } else if (appName == "Start") {
        Start *cstart = ui->windows->findChild<Start*>("Start");
        if (cstart->close()){
            cstart->deleteLater();
            ui->windows->removeTab(index);
        }
    } else if (appName == "Search") {
        search *csearch = ui->windows->findChild<search*>("search");
        if (csearch->close()){
            csearch->deleteLater();
            ui->windows->removeTab(index);
        }
    } else if (appName == "Help") {
        help *chelp = ui->windows->findChild<help*>("help");
        if (chelp->close()){
            chelp->deleteLater();
            ui->windows->removeTab(index);
        }
    }else if (appName == "Settings") {
        settings *csettings = ui->windows->findChild<settings*>("settings");
        if (csettings->close()){
            csettings->deleteLater();
            ui->windows->removeTab(index);
        }
    }
    else if (appName == "Core Paint") {
        corepaint *cpaint = ui->windows->findChild<corepaint*>("corepaint");
        if (cpaint->close()){
            cpaint->deleteLater();
            ui->windows->removeTab(index);
        }
    } else if (appName == "Core Player") {
        coreplayer *cplay = ui->windows->findChild<coreplayer*>("coreplayer");
        if (cplay->close()){
            cplay->deleteLater();
            ui->windows->removeTab(index);
        }
    } else if (appName == "CoreFM") {
        corefm *cfm = ui->windows->findChild<corefm*>("corefm");
        if (cfm->close()){
            cfm->deleteLater();
            ui->windows->removeTab(index);
        }
    } else if (appName == "Core Pad") {
        corepad *cpad = ui->windows->findChild<corepad*>("corepad");
        if (cpad->close()){
            cpad->deleteLater();
            ui->windows->removeTab(index);
        }
    } else if (appName == "Core Image") {
        coreimage *cimg = ui->windows->findChild<coreimage*>("coreimage");
        if (cimg->close()){
            cimg->deleteLater();
            ui->windows->removeTab(index);
        }
    } else if (appName == "Core Time") {
        coretime *ctim = ui->windows->findChild<coretime*>("coretime");
        if (ctim->close()){
            ctim->deleteLater();
            ui->windows->removeTab(index);
        }
    }

    if(ui->windows->count() == 0){
        on_start_clicked();
    }
}

void CoreBox::on_windows_currentChanged(int index)
{
    QString title = ui->windows->tabText(index);
    this->setWindowTitle(title);

    this->setWindowIcon(QIcon(appsIconPath(title)));
}

void CoreBox::closeCurrentTab() {
    on_windows_tabCloseRequested(ui->windows->currentIndex());
}

void CoreBox::on_maximizeButton_clicked()
{
    ui->restoreButton->setVisible(true);
    ui->maximizeButton->setVisible(false);
    this->setWindowState(Qt::WindowMaximized);
}

void CoreBox::on_restoreButton_clicked()
{
    ui->restoreButton->setVisible(false);
    ui->maximizeButton->setVisible(true);
    this->setWindowState(Qt::WindowNoState);
}

void CoreBox::on_minimizeButton_clicked()
{
    this->setWindowState(Qt::WindowMinimized);
}

void CoreBox::on_closeButton_clicked()
{
    this->close();
}

void CoreBox::doubleClicked()
{
  if (windowState().testFlag(Qt::WindowNoState)) {
    on_maximizeButton_clicked();
  } else if (windowState().testFlag(Qt::WindowMaximized)) {
    on_restoreButton_clicked();
  }
}

void CoreBox::changeEvent(QEvent *event)
{
  if (event->type() == QEvent::WindowStateChange) {
    if (windowState().testFlag(Qt::WindowNoState)) {
      on_restoreButton_clicked();
      event->ignore();
    } else if (windowState().testFlag(Qt::WindowMaximized)) {
      on_maximizeButton_clicked();
      event->ignore();
    }
  }
  event->accept();
}

void CoreBox::mousePressEvent(QMouseEvent *event)
{
    mousePressed = true;
    mousePos = event->globalPos();

//    if ( event->button() == Qt::LeftButton ){
//        mousePressed = true;
//        mousePos = event->globalPos();

//        if (ui->windows->tabBar()->underMouse())
//          wndPos = this->pos();
//    }

    if (ui->windows->tabBar()->underMouse())
      wndPos = this->pos();
}

void CoreBox::mouseMoveEvent(QMouseEvent *event)
{
    if (ui->windows->tabBar()->underMouse() && mousePressed)
      move(wndPos + (event->globalPos() - mousePos));
}

void CoreBox::mouseReleaseEvent(QMouseEvent *event)
{
  Q_UNUSED(event);
  mousePressed = false;
}

bool CoreBox::eventFilter(QObject *obj, QEvent *evt)
{
    if (obj == ui->windows && evt->type() == QEvent::MouseButtonDblClick){
        doubleClicked();
    }

    return QMainWindow::eventFilter(obj,evt);
}

//===========================WindowBar========End=================================================================

void CoreBox::closeEvent(QCloseEvent *event) {
    event->ignore();
    if (ui->windows->count() > 1) {
        for (int i = ui->windows->count() - 1; i >= 1; --i) {
            on_windows_tabCloseRequested(i);
        }
    }
    event->accept();
}

void CoreBox::on_bookmarks_clicked()
{
    for(int b=0; b<ui->windows->count(); ++b) {
        if("Bookmarks" == ui->windows->tabText(b)) {
            ui->windows->setCurrentIndex(b);
            return;
        }
    }
    tabEngine(6);
}

void CoreBox::on_dashboard_clicked()
{
    for(int d=0; d<ui->windows->count(); ++d) {
        if("DashBoard" == ui->windows->tabText(d)) {
            ui->windows->setCurrentIndex(d);
            return;
        }
    }
    tabEngine(5);
}

void CoreBox::on_settings_clicked()
{
    for(int s=0; s<ui->windows->count(); ++s) {
        if("Settings" == ui->windows->tabText(s)) {
            ui->windows->setCurrentIndex(s);
            return;
        }
    }
    tabEngine(10);
}

void CoreBox::on_start_clicked()
{
    for(int i= 0; i < ui->windows->count(); ++i) {
        if(ui->windows->tabText(i) == "Start") {
            ui->windows->setCurrentIndex(i);
            return;
        }
    }
    tabEngine(8);
}

void CoreBox::on_search_clicked()
{
    for(int i=0; i<ui->windows->count(); ++i) {
        if("Search" == ui->windows->tabText(i)) {
            ui->windows->setCurrentIndex(i);
            return;
        }
    }
    tabEngine(11);
}

void CoreBox::on_corepaint_clicked()
{
    int n = ui->windows->count();
    ui->windows->insertTab(n,new corepaint,QIcon(":/icons/CorePaint.svg"),"Core Paint");
    ui->windows->setCurrentIndex(n);
}

void CoreBox::on_corefm_clicked()
{
    int n = ui->windows->count();
    ui->windows->insertTab(n,new corefm,QIcon(":/icons/CoreFM.svg"),"CoreFM");
    ui->windows->setCurrentIndex(n);
}

void CoreBox::on_corepad_clicked()
{
    int n = ui->windows->count();
    ui->windows->insertTab(n,new corepad,QIcon(":/icons/CorePad.svg"),"Core Pad");
    ui->windows->setCurrentIndex(n);
}

void CoreBox::on_box_clicked()
{
    messageEngine("Thanks for using CoreBox", "Info");
}
