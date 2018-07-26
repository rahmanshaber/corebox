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

#include "screenwidget.h"


ScreenWidget::ScreenWidget(QWidget *parent) : QGraphicsView(parent)
{
    cScene = new QGraphicsScene;
    setScene(cScene);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    connect(this,SIGNAL(subscreenTaken()),this,SLOT(close()));
}

const QPixmap &ScreenWidget::screen() const
{
    return cScreen;
}

void ScreenWidget::drawBackground(QPainter *p, const QRectF &rect)
{
    p->setBrush(Qt::black);
    p->setPen(Qt::NoPen);
    p->drawRect(rect);
    p->setOpacity(0.4);
    p->drawPixmap(0,0, cScreen);
}

void ScreenWidget::resizeEvent(QResizeEvent *event)
{
    scene()->setSceneRect(QRect(0,0,event->size().width(), event->size().height()));
    QGraphicsView::resizeEvent(event);
}

void ScreenWidget::mouseDoubleClickEvent(QMouseEvent * event)
{
    Q_UNUSED(event);
    emit subscreenTaken();
}

void ScreenWidget::takeScreen()
{
    foreach (QScreen * s, qGuiApp->screens())
    {
        if (s->geometry().contains(QCursor::pos())){
            cScreen = s->grabWindow(0);
            cScreen = cScreen.copy(s->geometry());
        }
    }
    resize(cScreen.size());
    update();
    emit screenTaken();
}
