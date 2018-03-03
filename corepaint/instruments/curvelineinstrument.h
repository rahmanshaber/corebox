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

#ifndef CURVELINEINSTRUMENT_H
#define CURVELINEINSTRUMENT_H

#include "abstractinstrument.h"

#include <QtCore/QObject>
#include <QtCore/QPoint>


class CurveLineInstrument : public AbstractInstrument
{
    Q_OBJECT
public:
    explicit CurveLineInstrument(QObject *parent = 0);
    
    virtual void mousePressEvent(QMouseEvent *event, ImageArea &imageArea);
    virtual void mouseMoveEvent(QMouseEvent *event, ImageArea &imageArea);
    virtual void mouseReleaseEvent(QMouseEvent *event, ImageArea &imageArea);

protected:
    void paint(ImageArea &imageArea, bool isSecondaryColor = false, bool additionalFlag = false);

private:
    QPoint mFirstControlPoint, mSecondControlPoint;
    unsigned int mPointsCount : 2; /**< Chaneges from 0 to 2, so 2 bits is enough. */
};

#endif // CURVELINEINSTRUMENT_H
