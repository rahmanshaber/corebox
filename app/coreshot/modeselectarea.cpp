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

#include "modeselectarea.h"


ModeSelectArea::ModeSelectArea(QWidget *parent) : ScreenWidget(parent)
{
    setObjectName("Select Area Mode");
    setMouseTracking(true);
    cRectItem = new RectArea;
    cIsAreaEnable = false;
}

QPixmap ModeSelectArea::subscreen() const
{
    QRect zone = cRectItem->mapToScene(cRectItem->rect()).boundingRect().toRect();
    return screen().copy(zone);
}

void ModeSelectArea::mousePressEvent(QMouseEvent *event)
{
    if (!cIsAreaEnable) {
    cRectItem->setScreen(screen());
    cRectItem->setRect(0,0,2,2);
    scene()->addItem(cRectItem);
    cRectItem->setPos(event->pos());
    cIsAreaEnable = true;
    }
    ScreenWidget::mousePressEvent(event);
}

void ModeSelectArea::closeEvent(QCloseEvent *event)
{
    cIsAreaEnable = false;
    scene()->removeItem(cRectItem);
    ScreenWidget::closeEvent(event);
}

void ModeSelectArea::setAreaEnable(bool active)
{
    cIsAreaEnable = active;
}
