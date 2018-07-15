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

#ifndef ABSTRACTINSTRUMENT_H
#define ABSTRACTINSTRUMENT_H

#include <QObject>
#include <QMouseEvent>
#include <QImage>

QT_BEGIN_NAMESPACE
class ImageArea;
QT_END_NAMESPACE


class AbstractInstrument : public QObject
{
    Q_OBJECT

public:
    explicit AbstractInstrument(QObject *parent = 0);
    virtual ~AbstractInstrument(){}
    virtual void mousePressEvent(QMouseEvent *event, ImageArea &imageArea) = 0;
    virtual void mouseMoveEvent(QMouseEvent *event, ImageArea &imageArea) = 0;
    virtual void mouseReleaseEvent(QMouseEvent *event, ImageArea &imageArea) = 0;

protected:
    QPoint mStartPoint, mEndPoint; // Point for events.
    QImage mImageCopy;             // Image for storing copy of current image on imageArea, needed for some instruments.

    virtual void paint(ImageArea &imageArea, bool isSecondaryColor = false, bool additionalFlag = false) = 0;
    virtual void makeUndoCommand(ImageArea &imageArea);
    
};

#endif // ABSTRACTINSTRUMENT_H
