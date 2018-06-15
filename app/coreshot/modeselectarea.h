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

#ifndef MODESELECTAREA_H
#define MODESELECTAREA_H

#include "rectarea.h"
#include "screenwidget.h"

#include <QWidget>
#include <QDesktopWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>


class ModeSelectArea : public ScreenWidget
{
    Q_OBJECT

public:
    explicit ModeSelectArea(QWidget *parent = 0);
    virtual QPixmap subscreen() const;

protected:
    void mousePressEvent(QMouseEvent * event);
    void closeEvent(QCloseEvent * );

protected slots:
    void setAreaEnable(bool active);

private:
    bool cIsAreaEnable;
    RectArea * cRectItem;

};

#endif // MODESELECTAREA_H
