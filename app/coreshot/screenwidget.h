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

#ifndef SCREENWIDGET_H
#define SCREENWIDGET_H

#include <QWidget>
#include <QDesktopWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QScreen>
#include <QGuiApplication>
#include <QPainter>
#include <QGraphicsView>
#include <QtWidgets>


class ScreenWidget : public QGraphicsView
{
    Q_OBJECT

public:
    explicit ScreenWidget(QWidget *parent = 0);
    virtual QPixmap subscreen() const = 0;
    const QPixmap &screen() const;

    void drawBackground(QPainter * p, const QRectF& rect);
    void resizeEvent(QResizeEvent * event);

protected:
    void mouseDoubleClickEvent(QMouseEvent *);

public slots:
    virtual void takeScreen();

signals:
    void capture();
    void screenTaken();
    void subscreenTaken();

private:
    QPixmap cScreen;
    QGraphicsScene * cScene;
    QGraphicsPixmapItem * mBackground;

};

#endif // SCREENWIDGET_H
