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

#include "start/start.h"
#include "about/about.h"
#include "help/help.h"
#include "search/search.h"
#include "dashboard/dashboard.h"
#include "corepad/corepad.h"
#include "corefm/corefm.h"
#include "corepaint/corepaint.h"
#include "coreimage/coreimage.h"
#include "coreplayer/coreplayer.h"
#include "coreaction/coreaction.h"
#include "coretime/coretime.h"
#include "corepdf/corepdf.h"
#include "coreterminal/coreterminal.h"
#include "corerenamer/corerenamer.h"
#include "start/sessionsavedialog.h"


CoreBox::CoreBox(QWidget *parent) : QMainWindow(parent), ui(new Ui::CoreBox)
{
    ui->setupUi(this);

    //set a icon set for the whole app
    QIcon::setThemeName(sm.getThemeName());

    //setup framless window
    ui->windows->tabBar()->installEventFilter(this);
    ui->sidebar->installEventFilter(this);

    setWindowFlags(Qt::FramelessWindowHint |
      Qt::WindowSystemMenuHint | Qt::SubWindow);

    ui->restoreButton->setVisible(false);
//    ui->windows->setCornerWidget(ui->winbutn, Qt::BottomRightCorner);

    //make the resizeable window button
    QSizeGrip *sizeGrip = new QSizeGrip(this);
    sizeGrip->setStyleSheet("QWidget{background-color: #111111 ; width: 16px;height: 16px; background-image: url(:/icons/expand.svg); background-repeat: no-repeat ;background-position: center center ;}");
    ui->res->addWidget(sizeGrip);

    //setup window size and state
    if (sm.getBoxIsMaximize()) {
        on_maximizeButton_clicked();
    } else {
        int x = screensize().width()  * .8;
        int y = screensize().height() * .8;
        this->resize(x, y);
    }

    BookmarkManage bk;
    bk.checkBook();

    setupFolder(FolderSetup::BookmarkFolder);
    setupFolder(FolderSetup::DriveMountFolder);
    setupFolder(FolderSetup::TrashFolder);

}

CoreBox::~CoreBox()
{
    delete ui;
}

//===========================WindowBar========Start===============================================================

void CoreBox::tabEngine(AppsName i,const QString arg) // engine to open app in window
{
    int n = ui->windows->count();

    switch (i) {
    case CoreFM: {
        // Limit the openable tab to 10
        if (n > 10){
            messageEngine("Reached Window limite", MessageType::Warning);
            return;
        }

        corefm *cFM = new corefm();

        QString str = checkIsValidDir(arg);
        if (!str.isEmpty() || !str.isNull()) cFM->goTo(str);

        ui->windows->insertTab(n, cFM, QIcon(":/app/icons/app-icons/CoreFM.svg"), "CoreFM");
        ui->windows->setCurrentIndex(n);
        break;
    }
    case CoreImage: {
        // Limit the openable tab to 10
        if (n > 10){
            messageEngine("Reached Window limite", MessageType::Warning);
            return;
        }

        coreimage *cIMG = new coreimage();

        QString str = checkIsValidFile(arg);
        if (!str.isEmpty() || !str.isNull()) cIMG->loadFile(str);

        ui->windows->insertTab(n, cIMG, QIcon(":/app/icons/app-icons/CoreImage.svg"), "CoreImage");
        ui->windows->setCurrentIndex(n);
        break;
    }
    case CorePad: {
        int nn = filterEngine("CorePad");

        if (nn != 404) {
            ui->windows->setCurrentIndex(nn);
            corepad *cPad = static_cast<corepad*>(ui->windows->currentWidget());
            cPad->initializeNewTab(checkIsValidFile(arg));
        } else {
            // Limit the openable tab to 10
            if (n > 10){
                messageEngine("Reached Window limite", MessageType::Warning);
                return;
            }

            corepad *cPAD = new corepad();
            cPAD->openText(checkIsValidFile(arg));

            ui->windows->insertTab(n, cPAD, QIcon(":/app/icons/app-icons/CorePad.svg"), "CorePad");
            ui->windows->setCurrentIndex(n);
        }
        break;
    }
    case CorePaint: {
        int nn = filterEngine("CorePaint");

        if (nn != 404) {
            ui->windows->setCurrentIndex(nn);
            corepaint *cPaint = static_cast<corepaint*>(ui->windows->currentWidget());
            cPaint->initializeNewTab(true, arg);
        } else {
            // Limit the openable tab to 10
            if (n > 10){
                messageEngine("Reached Window limite", MessageType::Warning);
                return;
            }

            corepaint *cPAINT = new corepaint();

            const QString str = checkIsValidFile(arg);
            if (!str.isEmpty() || !str.isNull()) cPAINT->initializeNewTab(true, str);
            else cPAINT->initializeNewTab();

            ui->windows->insertTab(n, cPAINT, QIcon(":/app/icons/app-icons/CorePaint.svg"), "CorePaint");
            ui->windows->setCurrentIndex(n);
        }
        break;
    }
    case CorePlayer: {
        // Limit the openable tab to 10
        if (n > 10){
            messageEngine("Reached Window limite", MessageType::Warning);
            return;
        }

        coreplayer *cPLAYER = new coreplayer();

        const QString str = checkIsValidFile(arg);
        if (!str.isEmpty() || !str.isNull()) cPLAYER->openPlayer(str);

        ui->windows->insertTab(n, cPLAYER, QIcon(":/app/icons/app-icons/CorePlayer.svg"), "CorePlayer");
        ui->windows->setCurrentIndex(n);
        break;
    }
    case Dashboard: {
        int nn = filterEngine("DashBoard");

        if (nn != 404) ui->windows->setCurrentIndex(nn);
        else {
            // Limit the openable tab to 10
            if (n > 10){
                messageEngine("Reached Window limite", MessageType::Warning);
                return;
            }

            ui->windows->insertTab(n, new dashboard(), QIcon(":/app/icons/app-icons/DashBoard.svg"), "DashBoard");
            ui->windows->setCurrentIndex(n);
        }
        break;
    }
    case Bookmarks: {
        int nn = filterEngine("Bookmarks");

        if (nn != 404) ui->windows->setCurrentIndex(nn);
        else {
            // Limit the openable tab to 10
            if (n > 10){
                messageEngine("Reached Window limite", MessageType::Warning);
                return;
            }

            ui->windows->insertTab(n, new bookmarks(), QIcon(":/app/icons/app-icons/Bookmarks.svg"), "Bookmarks");
            ui->windows->setCurrentIndex(n);
        }
        break;
    }
    case About: {
        int nn = filterEngine("About");

        if (nn != 404) ui->windows->setCurrentIndex(nn);
        else {
            // Limit the openable tab to 10
            if (n > 10){
                messageEngine("Reached Window limite", MessageType::Warning);
                return;
            }
            ui->windows->insertTab(n, new about(), QIcon(":/app/icons/app-icons/About.svg"), "About");
            ui->windows->setCurrentIndex(n);
        }
        break;
    }
    case StartView: {
        int nn = filterEngine("Start");

        if (nn != 404) ui->windows->setCurrentIndex(nn);
        else {
            // Limit the openable tab to 10
            if (n > 10){
                messageEngine("Reached Window limite", MessageType::Warning);
                return;
            }

            ui->windows->insertTab(n, new Start(), QIcon(":/app/icons/app-icons/Start.svg"), "Start");
            ui->windows->setCurrentIndex(n);
        }
        break;
    }
    case Help: {
        int nn = filterEngine("Help");

        if (nn != 404) ui->windows->setCurrentIndex(nn);
        else {
            // Limit the openable tab to 10
            if (n > 10){
                messageEngine("Reached Window limite", MessageType::Warning);
                return;
            }

            ui->windows->insertTab(n, new help(), QIcon(":/app/icons/app-icons/Help.svg"), "Help");
            ui->windows->setCurrentIndex(n);
        }
        break;
    }
    case Settings: {
        int nn = filterEngine("Settings");

        if (nn != 404) ui->windows->setCurrentIndex(nn);
        else {
            // Limit the openable tab to 10
            if (n > 10){
                messageEngine("Reached Window limite", MessageType::Warning);
                return;
            }

            ui->windows->insertTab(n, new settings(), QIcon(":/app/icons/app-icons/Settings.svg"), "Settings");
            ui->windows->setCurrentIndex(n);
        }
        break;
    }
    case Search: {
        // Limit the openable tab to 10
        if (n > 10){
            messageEngine("Reached Window limite", MessageType::Warning);
            return;
        }

        search *ser = new search();

        const QString str = checkIsValidDir(arg);
        if (!str.isEmpty() || !str.isNull()) ser->setPath(str);

        ui->windows->insertTab(n, ser, QIcon(":/app/icons/app-icons/Search.svg"), "Search");
        ui->windows->setCurrentIndex(n);
        break;
    }
    case CoreTime: {
        int nn = filterEngine("CoreTime");

        if (nn != 404) ui->windows->setCurrentIndex(nn);
        else {
            // Limit the openable tab to 10
            if (n > 10){
                messageEngine("Reached Window limite", MessageType::Warning);
                return;
            }

            ui->windows->insertTab(n, new coretime, QIcon(":/app/icons/app-icons/CoreTime.svg"), "CoreTime");
            ui->windows->setCurrentIndex(n);
        }
        break;
    }
    case Corebox: {
        CoreBox *cBox = new CoreBox();
        cBox->show();
        cBox->tabEngine(StartView);
        break;
    }
    case CorePDF: {
        // Limit the openable tab to 10
        if (n > 10){
            messageEngine("Reached Window limite", MessageType::Warning);
            return;
        }

        corepdf *cPDF = new corepdf();

        const QString str = checkIsValidFile(arg);
        if (!str.isEmpty() || !str.isNull()) cPDF->openPdfFile(str);

        ui->windows->insertTab(n, cPDF, QIcon(":/app/icons/app-icons/CorePDF.svg"), "CorePDF");
        ui->windows->setCurrentIndex(n);
        break;
    }
    case CoreTerminal: {
        // Limit the openable tab to 10
        if (n > 10){
            messageEngine("Reached Window limite", MessageType::Warning);
            return;
        }

        QString workDir = arg;
        if (!arg.count())
            workDir = QDir::homePath();

        if (QFileInfo(workDir).isFile())
            workDir = QFileInfo(arg).path();

        coreterminal *trm = new coreterminal(workDir, "", this);
        ui->windows->insertTab(n, trm, QIcon(":/app/icons/app-icons/CoreTerminal.svg"), "CoreTerminal");
        ui->windows->setCurrentIndex(n);
        break;
    }
    case CoreRenamer: {
        // Limit the openable tab to 10
        if (n > 10){
            messageEngine("Reached Window limite", MessageType::Warning);
            return;
        }

        corerenamer *cFM = new corerenamer();

        const QString str = checkIsValidDir(arg);
        if (!str.isEmpty() || !str.isNull()) cFM->addPath(str);

        ui->windows->insertTab(n, cFM, QIcon(":/app/icons/app-icons/CoreRenemer.svg"), "CoreRenamer");
        ui->windows->setCurrentIndex(n);

        break;
    }
    default:
        break;
    }
}

void CoreBox::on_windows_tabCloseRequested(int index)
{
    const QString appName = ui->windows->tabBar()->tabText(index);
    if (appName == "Bookmarks") {
        bookmarks *cbook = static_cast<bookmarks*>(ui->windows->widget(index));
        if (cbook->close()){
            cbook->deleteLater();
            ui->windows->removeTab(index);
        }
    } else if (appName == "About") {
        about *cabout = static_cast<about*>(ui->windows->widget(index));
        if (cabout->close()){
            cabout->deleteLater();
            ui->windows->removeTab(index);
        }
    } else if (appName == "DashBoard") {
        dashboard *cdash = static_cast<dashboard*>(ui->windows->widget(index));
        if (cdash->close()){
            cdash->deleteLater();
            ui->windows->removeTab(index);
        }
    } else if (appName == "Start") {
        Start *cstart = static_cast<Start*>(ui->windows->widget(index));
        if (cstart->close()){
            cstart->deleteLater();
            ui->windows->removeTab(index);
        }
    } else if (appName == "Search") {
        search *csearch = static_cast<search*>(ui->windows->widget(index));
        if (csearch->close()){
            csearch->deleteLater();
            ui->windows->removeTab(index);
        }
    } else if (appName == "Help") {
        help *chelp = static_cast<help*>(ui->windows->widget(index));
        if (chelp->close()){
            chelp->deleteLater();
            ui->windows->removeTab(index);
        }
    } else if (appName == "Settings") {
        settings *csettings = static_cast<settings*>(ui->windows->widget(index));
        if (csettings->close()){
            csettings->deleteLater();
            ui->windows->removeTab(index);
        }
    }
    else if (appName == "CorePaint") {
        corepaint *cpaint = static_cast<corepaint*>(ui->windows->widget(index));
        if (cpaint->close()){
            cpaint->deleteLater();
            ui->windows->removeTab(index);
        }
    } else if (appName == "CorePlayer") {
        coreplayer *cplay = static_cast<coreplayer*>(ui->windows->widget(index));
        if (cplay->close()){
            cplay->deleteLater();
            ui->windows->removeTab(index);
        }
    } else if (appName == "CoreFM") {
        corefm *cfm = static_cast<corefm*>(ui->windows->widget(index));
        if (cfm->close()){
            cfm->deleteLater();
            ui->windows->removeTab(index);
        }
    } else if (appName == "CorePad") {
        corepad *cpad = static_cast<corepad*>(ui->windows->widget(index));
        if (cpad->close()){
            cpad->deleteLater();
            ui->windows->removeTab(index);
        }
    } else if (appName == "CoreImage") {
        coreimage *cimg = static_cast<coreimage*>(ui->windows->widget(index));
        if (cimg->close()){
            cimg->deleteLater();
            ui->windows->removeTab(index);
        }
    } else if (appName == "CoreTime") {
        coretime *ctim = static_cast<coretime*>(ui->windows->widget(index));
        if (ctim->close()){
            ctim->deleteLater();
            ui->windows->removeTab(index);
        }
    } else if (appName == "CorePDF") {
        corepdf *cpdf = static_cast<corepdf*>(ui->windows->widget(index));
//        cpdf->eclose();
        if (cpdf->close()){
            cpdf->deleteLater();
            ui->windows->removeTab(index);
        }
    } else if (appName == "CoreTerminal") {
        coreterminal *ctrm = static_cast<coreterminal*>(ui->windows->widget(index));
        if (ctrm->close()){
            ctrm->deleteLater();
            ui->windows->removeTab(index);
        }
    } else if (appName == "CoreRenamer") {
        corerenamer *cren = static_cast<corerenamer*>(ui->windows->widget(index));
        if (cren->close()){
            cren->deleteLater();
            ui->windows->removeTab(index);
        }
    }

    if(ui->windows->count() == 0){
        on_start_clicked();
    }
}

int CoreBox::filterEngine(QString name) // engine for find if a app is opened
{
    for(int i= 0; i < ui->windows->count(); ++i) {
        if(ui->windows->tabText(i) == name) {
            return i ;
        }
    }
    return 404;
}

void CoreBox::on_windows_currentChanged(int index) // set window title related to current selected app
{
    QString title = ui->windows->tabText(index);
    this->setWindowTitle(title);
    this->setWindowIcon(appsIcon(title));

    if (ui->windows->currentWidget())
        ui->windows->currentWidget()->setFocus();
}

void CoreBox::on_windows_tabBarClicked(int index) // reload the apps if related app is clicked
{
    QString appName = ui->windows->tabBar()->tabText(index);

    if (appName == "Bookmarks") {
        bookmarks *cbook = ui->windows->findChild<bookmarks*>("bookmarks");
        cbook->reload();
    } else if (appName == "Start") {
        Start *cstart = ui->windows->findChild<Start*>("Start");
        cstart->reload();
    }
}

void CoreBox::closeCurrentTab()
{
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
    if(ui->windows->tabBar()->underMouse() || ui->sidebar->underMouse() && event->buttons()== Qt::LeftButton){
        isMouseDown = true;
        isLeftDown = true;
        mousePos = event->globalPos();
        wndPos = this->pos();
//        qDebug() << "Left mouse clicked";
    }
//    else if (event->buttons()== Qt::RightButton){
//        isMouseDown = true;
//        isRightDown = true;
//        mousePos = event->globalPos();
//        wndPos = this->pos();
//        qDebug() << "Right mouse clicked";
//    }
}

void CoreBox::mouseMoveEvent(QMouseEvent *event)
{
    if(isMouseDown==true){
//        int mousePointx = wndPos.x() + (event->globalX() - mousePos.x());
//        int mousePointy = wndPos.y() + (event->globalY() - mousePos.y());

        if ((ui->windows->tabBar()->underMouse() || ui->sidebar->underMouse()) && isLeftDown == true){
            move(wndPos + (event->globalPos() - mousePos));
//            qDebug() << "Left mouse clicked";
        }
//        else if (isRightDown == true){
//            resize((mousePointx),(mousePointy));
//            qDebug() << "Right mouse clicked";
//        }
    }
}

void CoreBox::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    isMouseDown = false;
    isLeftDown = false;
    isRightDown = false;
}

bool CoreBox::eventFilter(QObject *obj, QEvent *evt)
{
    if (obj == ui->sidebar && evt->type() == QEvent::MouseButtonDblClick){
        doubleClicked();
    }

    return QMainWindow::eventFilter(obj,evt);
}

//===========================WindowBar========End=================================================================

void CoreBox::closeEvent(QCloseEvent *event)
{
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
    tabEngine(Bookmarks);
}

void CoreBox::on_dashboard_clicked()
{
    tabEngine(Dashboard);
}

void CoreBox::on_settings_clicked()
{
    tabEngine(Settings);
}

void CoreBox::on_start_clicked()
{
    tabEngine(StartView);
}

void CoreBox::on_search_clicked()
{
    tabEngine(Search);
}

void CoreBox::on_corepaint_clicked()
{
    tabEngine(CorePaint);
}

void CoreBox::on_corefm_clicked()
{
    tabEngine(CoreFM);
}

void CoreBox::on_corepad_clicked()
{
    tabEngine(CorePad);
}

void CoreBox::on_box_clicked()
{
    // Function from globalfunctions.cpp
    messageEngine("Thanks for using CoreBox\nVersion 2.2", MessageType::Info);
}

void CoreBox::on_saveSession_clicked()
{
    QString sName = "";
    sessionSaveDialog *ssd = new sessionSaveDialog(this);
    connect(ssd, &sessionSaveDialog::nameOk, [this, ssd, &sName]() {
        sName = ssd->sName;
        ssd->close();
    });
    ssd->exec();

    if (!sName.count()) {
        messageEngine("Session Name empty\nNot Saved", MessageType::Info);
        return;
    }

    QSettings session(QDir::homePath() + "/.config/coreBox/Sessions", QSettings::IniFormat);

    if (session.childGroups().count() > 15) {
        messageEngine("Session save limit reached", MessageType::Warning);
        return;
    }

    session.beginGroup(QDate::currentDate().toString("dd.MM.yyyy"));
    session.beginGroup(sName);
    for (int i = 0; i < ui->windows->count(); i++) {
        QThread::currentThread()->msleep(1);
        QString key = QTime::currentTime().toString("hh.mm.ss.zzz");
        QString value = "";
        switch (nameToInt(ui->windows->tabText(i))) {
        case CoreImage: {
            session.beginGroup("CoreImage");
            value = static_cast<coreimage*>(ui->windows->widget(i))->currentImagePath;
            session.setValue(key, value);
            session.endGroup();
            break;
        }
        case CorePDF: {
            session.beginGroup("CorePDF");
            value = static_cast<corepdf*>(ui->windows->widget(i))->workFilePath;
            session.setValue(key, value);
            session.endGroup();
            break;
        }
        case CorePlayer: {
            session.beginGroup("CorePlayer");
            value = static_cast<coreplayer*>(ui->windows->widget(i))->workFilePath();
            session.setValue(key, value);
            session.endGroup();
            break;
        }
        case CoreRenamer: {
            session.beginGroup("CoreRenamer");
            value = static_cast<corerenamer*>(ui->windows->widget(i))->workFilePath;
            session.setValue(key, value);
            session.endGroup();
            break;
        }
        case CoreTerminal: {
            session.beginGroup("CoreTerminal");
            value = static_cast<coreterminal*>(ui->windows->widget(i))->currentWorkingDirectory();
            session.setValue(key, value);
            session.endGroup();
            break;
        }
        case Search: {
            session.beginGroup("Search");
            value = static_cast<search*>(ui->windows->widget(i))->workFilePath();
            session.setValue(key, value);
            session.endGroup();
            break;
        }
        case CoreFM: {
            session.beginGroup("CoreFM");
            corefm *cfm = static_cast<corefm*>(ui->windows->widget(i));
            for (int j = 0; j < cfm->tabsCount(); j++){
                QThread::currentThread()->msleep(1);
                // Created the key again for the time for every single page
                key = QTime::currentTime().toString("hh.mm.ss.zzz");

                // As corefm has so many current path variable (don't know but guess)
                // Just added a function to get the current page path
                value = cfm->gCurrentPath(j);
                session.setValue(key, value);
            }
            session.endGroup();
            break;
        }
        case CorePad: {
            session.beginGroup("CorePad");
            corepad *cpad = static_cast<corepad*>(ui->windows->widget(i));
            for (int j = 0; j < cpad->tabsCount(); j++) {
                QThread::currentThread()->msleep(1);
                // Created the key again for the time for every single page
                key = QTime::currentTime().toString("hh.mm.ss.zzz");
                value = cpad->currentFilePath(j);
                session.setValue(key, value);
            }
            session.endGroup();
            break;
        }
        case CorePaint: {
            session.beginGroup("CorePaint");
            corepaint *cpaint = static_cast<corepaint*>(ui->windows->widget(i));
            for (int j = 0; j < cpaint->tabsCount(); j++) {
                QThread::currentThread()->msleep(1);
                // Created the key again for the time for every single page
                key = QTime::currentTime().toString("hh.mm.ss.zzz");
                value = cpaint->getImageAreaByIndex(j)->mFilePath;
                session.setValue(key, value);
            }
            session.endGroup();
            break;
        }
        default:
            session.beginGroup(ui->windows->tabText(i));
            value = "";
            session.setValue(key, value);
            session.endGroup();
            break;
        }
    }
    session.endGroup();
    session.endGroup();

    messageEngine("Session Saved Successfully", MessageType::Info);
}

