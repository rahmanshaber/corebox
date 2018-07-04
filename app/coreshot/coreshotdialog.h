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

#ifndef CORESHOTDIALOG_H
#define CORESHOTDIALOG_H

#include "coreshot/screenwidget.h"
#include "coreshot/modeselectarea.h"
#include "coreshot/modefullscreen.h"
#include "coreshot/previewwidget.h"
#include "coreshot/coreshot.h"

#include <QWidget>
#include <QPixmap>
#include <QX11Info>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <QMimeDatabase>

#include "corebox/globalfunctions.h"


namespace Ui {
class coreshotdialog;
}

class coreshotdialog : public QWidget
{
    Q_OBJECT

public:
    explicit coreshotdialog(QWidget *parent = 0);
    ~coreshotdialog();

private:
    Ui::coreshotdialog *ui;
    QPixmap m_pixmap;

    WId getActiveWindowId();
    QRect getWindowFrame(WId wid);

    void passToShotPreview();
    void startsetup();
    ModeSelectArea *selcArea;
    ModeFullscreen *fullSc;

private slots:
    void shootActiveWindow();
    void shootFullScreen();
    void shootSelectArea();

};

#endif // CORESHOTDIALOG_H
