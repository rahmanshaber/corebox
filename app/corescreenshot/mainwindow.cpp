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

#include "mainwindow.h"

#include <QFileDialog>
#include <QClipboard>
#include <QSpinBox>
#include <QX11Info>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    cPreview  = new PreviewWidget();
    cShot     = new QPushButton("Take a new screenshot");
    cSave     = new QPushButton("Save");
    cCancel   = new QPushButton("Cancel");
    cCorePaint= new QPushButton("Send to CorePaint");
    cSaveAs   = new QPushButton("Save_As");
    cType     = new QComboBox();
    cdelaySec = new QSpinBox();
    cdelaylbl = new QLabel("Delay (sec) : ");
    cLable    = new QLabel("Capture Type:");

    addSnapMode(new ModeSelectArea());
    addSnapMode(new ModeFullscreen());

    QHBoxLayout * HLayout1 = new QHBoxLayout();
    QHBoxLayout * HLayout2  = new QHBoxLayout();
    QVBoxLayout * mainLayout = new QVBoxLayout();

    cPreview->setFrameShape(QFrame::Box	);
    cPreview->setFrameShadow(QFrame::Sunken);
    cPreview->setMinimumSize(528,297);                                   // screen ratio = 16:9
    cType->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    cShot->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Minimum);

    HLayout1->addWidget(cLable);
    HLayout1->addWidget(cType);
    HLayout1->addStretch();
    HLayout1->addWidget(cdelaylbl);
    HLayout1->addWidget(cdelaySec);
    HLayout1->addStretch();
    HLayout1->addWidget(cShot);

    HLayout2->addStretch();
    HLayout2->addWidget(cCorePaint);
    HLayout2->addWidget(cSave);
    HLayout2->addWidget(cSaveAs);
    HLayout2->addWidget(cCancel);

    mainLayout->addWidget(cPreview);
    mainLayout->addStretch();
    mainLayout->addLayout(HLayout1);
    mainLayout->addLayout(HLayout2);

    setLayout(mainLayout);
    cSave->setVisible(false);
    cSaveAs->setVisible(false);
    cCorePaint->setVisible(false);

    files = "";

    connect(cType, SIGNAL(currentIndexChanged(int)), this, SLOT(on_cType_IndexChanged(int)));
    connect(cShot,SIGNAL(clicked()),this,SLOT(waitAndTakeScreen()));
    connect(cCorePaint, SIGNAL(clicked()), this, SLOT(toCorePaint()));
    connect(cSave,SIGNAL(clicked()),this,SLOT(save()));
    connect(cSaveAs,SIGNAL(clicked()),this,SLOT(saveAs()));
    connect(cCancel,SIGNAL(clicked()),this,SLOT(close()));

    on_cType_IndexChanged(0);
    this->setWindowIcon(QIcon(":/icons/CoreShot.svg"));
    this->setWindowTitle("CoreShot");
    setStyleSheet("QWidget{background-color: #2A2A2A;color: #ffffff;border: 1px #2A2A2A;padding: 5px 5px 5px 5px;}");

    QString p = "QPushButton{border: 1px solid rgb(67, 67, 67);padding: 3px 2px 3px 2px;}QPushButton:hover {background-color: rgb(48, 140, 198)}QPushButton:checked{background-color:rgb(48, 140, 198);}";

    cPreview->setStyleSheet(p);
    cShot->setStyleSheet(p);
    cSave->setStyleSheet(p);
    cSaveAs->setStyleSheet(p);
    cCancel->setStyleSheet(p);
    cType->setStyleSheet(p);
    cCorePaint->setStyleSheet(p);

    //
    // Add Active window option
    cType->addItem("Active Window");
    //
}

MainWindow::~MainWindow()
{
    delete cPreview;
    delete cShot;
    delete cSave;
    delete cSaveAs;
    delete cCancel;
    delete cType;
    delete cCorePaint;
    delete cLable;
}

void MainWindow::waitAndTakeScreen()
{
    this->setWindowState(Qt::WindowMinimized);
    QTimer::singleShot(200,this,SLOT(takeScreen()));
}

void MainWindow::takeScreen()
{
    int index = cType->currentIndex();
    if (index == 2) {
        QTimer::singleShot(cdelaySec->value() * 1000, this, &MainWindow::shootActiveWindow);
        return;
    }
    ScreenWidget * snapWidget = cSnapWidgets.at(index);
    if (index == 0) {
        snapWidget->showFullScreen();
        snapWidget->takeScreen();
    }
    if (index == 1) {
        delaying(cdelaySec->value());
    }
}

void MainWindow::delaying(int secs)
{
    QTimer::singleShot(secs * 1000, this, SLOT(emitToS()));
}

void MainWindow::emitToS()
{
    ScreenWidget * snapWidget = cSnapWidgets.at(cType->currentIndex());
    snapWidget->takeScreen();
    emit snapWidget->subscreenTaken();
}

void MainWindow::save()
{
    QString fileName = sm.getSCSaveLocation();
    QFile file(fileName + "/Screenshot_" + QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss") + ".png");
    cPreview->originalPixmap().save(&file, "PNG");
    file.close();
    files = fileName;
    messageEngine("Screenshot Saved", "Info");
    this->close();
}

void MainWindow::saveAs()
{
    QString filename = QFileDialog::getSaveFileName(this,tr("Save Screenshot"),sm.getSCSaveLocation(), tr("Image (*.png)"));
    QFile file(filename + ".png");

    cPreview->originalPixmap().save(&file, "PNG");

    file.close();
    files = filename;
    messageEngine("Screenshot Saved", "Info");
    this->close();
}

void MainWindow::addSnapMode(ScreenWidget *widget)
{
    cSnapWidgets.append(widget);
    cType->addItem(widget->objectName());
    connect(widget,SIGNAL(subscreenTaken()),this,SLOT(setPreview()));
}

void MainWindow::toCorePaint()
{
    QString fileName = sm.getSCSaveLocation() + "/Screenshot_" + QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss") + ".png";
    QFile file(fileName);
    cPreview->originalPixmap().save(&file, "PNG");
    file.close();
    files = fileName;
    CoreBox *cBox = new CoreBox();
    cBox->tabEngine(CorePaint, files);
    this->close();
    cBox->show();
}

void MainWindow::setPreview()
{
    ScreenWidget * snapWidget = qobject_cast<ScreenWidget*>(sender());
    cPreview->setOriginalPixmap(snapWidget->subscreen());
    cShot->setFocus();

    cSave->setVisible(true);
    cSaveAs->setVisible(true);
    cCorePaint->setVisible(true);
    this->setWindowState(Qt::WindowNoState);
}

void MainWindow::on_cType_IndexChanged(int index) {
    if (index == 0) {
        cdelaylbl->setVisible(false);
        cdelaySec->setVisible(false);
    } else if (index == 1) {
        cdelaylbl->setVisible(true);
        cdelaySec->setVisible(true);
    } else if (index == 2) {
        cdelaylbl->setVisible(true);
        cdelaySec->setVisible(true);
    }
}

void MainWindow::shootActiveWindow() {
    // Current window rectangle.
    QRect rect = getWindowFrame(getActiveWindowId());
    // Current whole screen.
    QScreen *screen = QGuiApplication::primaryScreen();

    auto cwposX = rect.topLeft().x();
    auto cwposY = rect.topLeft().y();
    auto cwHeight = rect.topLeft().y() + rect.height();
    auto cwWidth = rect.topLeft().x() + rect.width();
    auto scHeight = screen->size().height();
    auto scWidth = screen->size().width();
    auto scposX = screen->availableGeometry().x();
    auto scposY = screen->availableGeometry().y();

    if (cwHeight > scHeight) {
        cwHeight = rect.height() - (cwHeight - screen->availableSize().height());
        if (cwposY < 0) {
            cwposY = scposY;
            cwHeight = cwHeight + rect.y();
        }
    } else {
        cwHeight = rect.height();
        if (cwposY < 0) {
            cwposY = scposY;
            cwHeight = rect.y() + rect.height() - cwposY;
        }
    }

    if (cwWidth > scWidth) {
        cwWidth = rect.width() - (cwWidth - screen->availableSize().width());
        if (cwposX < 0) {
            cwposX = scposX;
            cwWidth = cwWidth + rect.x();
        }
    } else {
        cwWidth = rect.width();
        if (cwposX < 0) {
            cwposX = scposX;
            cwWidth = rect.x() + rect.width() - cwposX;
        }
    }
    auto pixmap = screen->grabWindow(QApplication::desktop()->winId(),
                                     cwposX, cwposY, cwWidth, cwHeight);

    cPreview->setOriginalPixmap(pixmap);

    cSave->setVisible(true);
    cSaveAs->setVisible(true);
    cCorePaint->setVisible(true);
    this->setWindowState(Qt::WindowNoState);
}

WId MainWindow::getActiveWindowId()
{
    WId root = WId(QX11Info::appRootWindow());
    Atom atom = XInternAtom(QX11Info::display(), "_NET_ACTIVE_WINDOW", false);
    unsigned long type, resultLen, rest;
    int format;
    WId result = 0;
    unsigned char* data = NULL;
    if(XGetWindowProperty(QX11Info::display(), root, atom, 0, 1, false,
        XA_WINDOW, &type, &format, &resultLen, &rest, &data) == Success) {
      result = *reinterpret_cast<long*>(data);
      XFree(data);
    }
    return result;
}

QRect MainWindow::getWindowFrame(WId wid)
{
    QRect result;
    XWindowAttributes wa;
    if(XGetWindowAttributes(QX11Info::display(), wid, &wa)) {
      Window child;
      int x, y;
      // translate to root coordinate
      XTranslateCoordinates(QX11Info::display(), wid, wa.root, 0, 0, &x, &y, &child);
      //qDebug("%d, %d, %d, %d", x, y, wa.width, wa.height);
      result.setRect(x, y, wa.width, wa.height);

      // get the frame widths added by the window manager
      Atom atom = XInternAtom(QX11Info::display(), "_NET_FRAME_EXTENTS", false);
      unsigned long type, resultLen, rest;
      int format;
      unsigned char* data = NULL;
      if(XGetWindowProperty(QX11Info::display(), wid, atom, 0, LONG_MAX, false,
        XA_CARDINAL, &type, &format, &resultLen, &rest, &data) == Success) {
      }
      if(data) { // left, right, top, bottom
        long* offsets = reinterpret_cast<long*>(data);
        result.setLeft(result.left() - offsets[0]);
        result.setRight(result.right() + offsets[1]);
        result.setTop(result.top() - offsets[2]);
        result.setBottom(result.bottom() + offsets[3]);
        XFree(data);
      }
    }
    return result;
}
