#include "coreshotdialog.h"
#include "coreshot/coreshot.h"

#include <QTimer>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QX11Info>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

CoreShotDialog::CoreShotDialog(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);

    QHBoxLayout *hl = new QHBoxLayout();
    hl->setContentsMargins(0, 0, 0, 0);

    QPushButton *cancel = new QPushButton("X");
    QPushButton *capSc = new QPushButton("Capture Screen");
    QPushButton *capWi = new QPushButton("Capture Window");
    QPushButton *capSe = new QPushButton("Capture Selection");

    QSpinBox *delay = new QSpinBox();
    delay->setValue(0);

    cancel->setMinimumSize(20, 20);
    capSc->setMinimumSize(50, 40);
    capWi->setMinimumSize(50, 40);
    capSe->setMinimumSize(50, 40);
    delay->setMinimumSize(25, 40);

    hl->addWidget(cancel);
    hl->addWidget(capSc);
    hl->addWidget(capWi);
    hl->addWidget(capSe);
    hl->addWidget(delay);

    setLayout(hl);
    //resize(230, 40);

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

    connect(capSc, &QPushButton::clicked, [this, delay]() {
        this->setWindowState(Qt::WindowMinimized);
        QTimer::singleShot(delay->value() * 1000, this, SLOT(shootFullScreen()));
    });

    connect(capWi, &QPushButton::clicked, [this, delay]() {
        this->setWindowState(Qt::WindowMinimized);
        QTimer::singleShot(delay->value() * 1000, this, SLOT(shootActiveWindow()));
    });

    connect(capSe, &QPushButton::clicked, [this, delay]() {
        this->setWindowState(Qt::WindowMinimized);
        QTimer::singleShot(200, this, SLOT(shootSelectArea()));
    });

    connect(cancel, &QPushButton::clicked, this, &CoreShotDialog::close);

}

void CoreShotDialog::passToShotPreview()
{
    coreshot *shootP = new coreshot();
    shootP->setPixmap(m_pixmap);
    shootP->show();
    this->close();
}

void CoreShotDialog::shootFullScreen()
{
    fullSc->takeScreen();
    emit fullSc->subscreenTaken();
}

void CoreShotDialog::shootSelectArea()
{
    selcArea->showFullScreen();
    selcArea->takeScreen();
}

void CoreShotDialog::shootActiveWindow()
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

//    cPreview->setOriginalPixmap(pixmap);
    //ui->shotPreview->setPixmap(pixmap);
    m_pixmap = pixmap;

    this->setWindowState(Qt::WindowNoState);
    passToShotPreview();
}

WId CoreShotDialog::getActiveWindowId()
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

QRect CoreShotDialog::getWindowFrame(WId wid)
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
