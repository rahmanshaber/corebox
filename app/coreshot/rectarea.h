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

#ifndef RECTAREA_H
#define RECTAREA_H

#include <QGraphicsRectItem>
#include <QGraphicsSceneMoveEvent>
#include <QPainter>


class RectArea : public QGraphicsRectItem
{

public:
    explicit RectArea(QGraphicsItem *parent = 0);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    QRect cornerToRect(const QPoint &p, int size = 4);
    QRectF boundingRect() const;

    void setScreen(const QPixmap& pix);

    void mousePressEvent(QGraphicsSceneMouseEvent*);
    void mouseMoveEvent(QGraphicsSceneMouseEvent*);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent*);

protected:
    QList<QRectF> corners();

    QRectF topLeftCorner();
    QRectF topMiddleCorner();
    QRectF topRightCorner();
    QRectF middleRightCorner();
    QRectF middleLeftCorner();
    QRectF bottomLeftCorner();
    QRectF bottomMiddleCorner();
    QRectF bottomRightCorner();

private:
    int cCornerSize;
    bool cIsMoving;
    int cCurrentCorner;
    QRectF mOldRect;
    QPixmap cScreen;

};

#endif // RECTAREA_H
