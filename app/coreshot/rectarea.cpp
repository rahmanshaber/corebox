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

#include "rectarea.h"


RectArea::RectArea(QGraphicsItem *parent) : QGraphicsRectItem(parent)
{
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setBrush(QBrush(Qt::white));
    setSelected(true);
    cIsMoving = false;
    cCurrentCorner = -1;

    setRect(0,0, 200, 200);
    cCornerSize = 10;
}

void RectArea::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setPen(QPen(Qt::white));
    painter->setOpacity(1);

    QRectF zone = mapToScene(rect()).boundingRect();

    painter->drawPixmap(rect(), cScreen, zone);

    if (!cIsMoving) {
        painter->setPen(QPen(Qt::white));
        painter->setBrush(QColor(128,128,128,200));

        foreach (QRectF corner, corners())
            painter->drawRect(corner);
    }
}

QRectF RectArea::boundingRect() const
{
    return QGraphicsRectItem::boundingRect();
}

void RectArea::setScreen(const QPixmap &pix)
{
    cScreen = pix;
}

void RectArea::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF mouse = mapFromScene(event->scenePos());

    if (event->button() == Qt::LeftButton){
        cCurrentCorner = 0;
        foreach (QRectF corner, corners())
        {
            if (corner.contains(mouse))
            {
                cIsMoving = true;
                mOldRect = rect();
                return;
            }
            ++cCurrentCorner;
        }
    }

    cIsMoving = false;
    cCurrentCorner = -1;

    QGraphicsRectItem::mousePressEvent(event);
}

void RectArea::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    if (cIsMoving)
    {
        QPointF mouse = mapFromScene(event->scenePos());
        QRectF r = mOldRect;

        switch (cCurrentCorner)
        {
        case 0: r.setTopLeft(mouse);break;
        case 1: r.setTop(mouse.y());break;
        case 2: r.setTopRight(mouse);break;
        case 3: r.setLeft(mouse.x());break;
        case 4: r.setRight(mouse.x());break;
        case 5: r.setBottomLeft(mouse);break;
        case 6: r.setBottomRight(mouse);break;
        case 7: r.setBottom(mouse.y());break;
        }
        setRect(r.normalized());
    }
    else
        QGraphicsRectItem::mouseMoveEvent(event);
}

void RectArea::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    cIsMoving = false;
    update();

    QGraphicsRectItem::mouseReleaseEvent(event);
}

QList<QRectF> RectArea::corners()
{
    QList<QRectF> list;
    list.append(topLeftCorner());
    list.append(topMiddleCorner());
    list.append(topRightCorner());
    list.append(middleRightCorner());
    list.append(middleLeftCorner());
    list.append(bottomLeftCorner());
    list.append(bottomRightCorner());
    list.append(bottomMiddleCorner());

    return list;
}

QRectF RectArea::topLeftCorner()
{
    QRectF r = QRectF(0,0, cCornerSize, cCornerSize);
    r.moveTo(rect().topLeft());
    return r;
}

QRectF RectArea::topMiddleCorner()
{
    QRectF r = QRectF(0,0, cCornerSize, cCornerSize);
    r.moveCenter(QPoint(rect().center().x(), rect().y() + cCornerSize/2));
    return r;
}

QRectF RectArea::topRightCorner()
{
    QRectF r = QRectF(0,0, cCornerSize, cCornerSize);
    r.moveTo(rect().topRight() - QPoint(cCornerSize+1,0));
    return r;
}

QRectF RectArea::middleRightCorner()
{
    QRectF r = QRectF(0,0, cCornerSize, cCornerSize);
    r.moveTo(rect().x(), rect().center().y() - cCornerSize/2);
    return r;
}

QRectF RectArea::middleLeftCorner()
{
    QRectF r = QRectF(0,0, cCornerSize, cCornerSize);
    r.moveTo(rect().right() - cCornerSize-1, rect().center().y()- cCornerSize/2);
    return r;
}

QRectF RectArea::bottomLeftCorner()
{
    QRectF r = QRectF(0,0, cCornerSize, cCornerSize);
    r.moveTo(rect().left(), rect().bottom() - cCornerSize-1);
    return r;
}

QRectF RectArea::bottomMiddleCorner()
{
    QRectF r = QRectF(0,0, cCornerSize, cCornerSize);
    r.moveCenter(QPointF(rect().center().x(), rect().bottom()-cCornerSize/2 -1));
    return r;
}

QRectF RectArea::bottomRightCorner()
{
    QRectF r = QRectF(0,0, cCornerSize, cCornerSize);
    r.moveTo(rect().right()-cCornerSize-1, rect().bottom() - cCornerSize -1);
    return r;
}
