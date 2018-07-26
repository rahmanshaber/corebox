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

#include "coreshotdialog.h"
#include "ui_coreshotdialog.h"

#include "corebox/globalfunctions.h"

coreshotdialog::coreshotdialog(QWidget *parent) :QWidget(parent),ui(new Ui::coreshotdialog)
{
    ui->setupUi(this);

    // set stylesheet from style.qrc
    setStyleSheet(getStylesheetFileContent(":/appStyle/style/CoreShotDialog.qss"));

    startsetup();

    // Set window as a framless
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint );

    // Set the window position
    int mx = (screensize().width()/2) - ((this->width()/2) + 10);
    int my = screensize().height() - (this->height() + 10);
    this->move(mx, my);
}

coreshotdialog::~coreshotdialog()
{
    delete ui;
}

void coreshotdialog::startsetup()
{
    selcArea = new ModeSelectArea();
    fullSc = new ModeFullscreen();

    connect(selcArea, &ModeSelectArea::subscreenTaken, [this]() {
        m_pixmap = selcArea->subscreen();
        passToShotPreview();
    });

    connect(fullSc, &ModeFullscreen::subscreenTaken, [this]() {
        m_pixmap = fullSc->subscreen();
        passToShotPreview();
    });

    connect(ui->captureScreen, &QPushButton::clicked, [this]() {
        this->setWindowState(Qt::WindowMinimized);
        if (windowState() == Qt::WindowMinimized)
            QTimer::singleShot((ui->delay->value() * 1000) + 200, this, SLOT(shootFullScreen()));
    });

    connect(ui->captureWindow, &QPushButton::clicked, [this]() {
        this->setWindowState(Qt::WindowMinimized);
        if (windowState() == Qt::WindowMinimized)
            QTimer::singleShot((ui->delay->value() * 1000) + 200, this, SLOT(shootActiveWindow()));
    });

    connect(ui->captureSelection, &QPushButton::clicked, [this]() {
        this->setWindowState(Qt::WindowMinimized);
        if (windowState() == Qt::WindowMinimized)
            QTimer::singleShot(200, this, SLOT(shootSelectArea()));
    });

    connect(ui->cancel, &QPushButton::clicked, this, &coreshotdialog::close);
}

void coreshotdialog::passToShotPreview()
{
    coreshot *shootP = new coreshot();
    shootP->setPixmap(m_pixmap);
    shootP->show();
    deleteLater();
}

void coreshotdialog::shootFullScreen()
{
    fullSc->takeScreen();
    emit fullSc->subscreenTaken();
}

void coreshotdialog::shootSelectArea()
{
    selcArea->showFullScreen();
    selcArea->takeScreen();
}

void coreshotdialog::shootActiveWindow()
{
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

    m_pixmap = pixmap;

    this->setWindowState(Qt::WindowNoState);
    passToShotPreview();
}

WId coreshotdialog::getActiveWindowId()
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

QRect coreshotdialog::getWindowFrame(WId wid)
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
