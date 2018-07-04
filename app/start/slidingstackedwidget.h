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

#ifndef SLIDINGSTACKEDWIDGET_H
#define SLIDINGSTACKEDWIDGET_H

#include <QWidget>
#include <QDebug>
#include <QEasingCurve>
#include <QStackedWidget>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>


class slidingStackedWidget : public QStackedWidget
{
    Q_OBJECT

public:
    slidingStackedWidget(QWidget *parent);
    ~slidingStackedWidget();

public:
    // This enumeration is used to define the animation direction
    enum t_direction {
        LEFT2RIGHT,
        RIGHT2LEFT,
        TOP2BOTTOM,
        BOTTOM2TOP,
        AUTOMATIC
    };

public slots:
    void setSpeed(int speed); // animation duration in milliseconds
    void setAnimation(const QEasingCurve::Type animationtype); // check out the QEasingCurve documentation for different styles
    void setVerticalMode(bool vertical = true);

    void slideInNext();
    void slideInPrev();
    void slideInIdx(int idx, t_direction direction = AUTOMATIC);

signals:
    void animationFinished();

private slots:
    void animationDoneSlot();

private:
    void slideInWgt(QWidget *widget, t_direction direction = AUTOMATIC);

    enum QEasingCurve::Type animationtype;
    int speed;
    bool vertical;
    int now;
    int next;
    QPoint pnow;
    bool active;
};

#endif // SLIDINGSTACKEDWIDGET_H
