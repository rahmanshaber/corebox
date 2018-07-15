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

#ifndef ABSTRACTSELECTION_H
#define ABSTRACTSELECTION_H

#include "abstractinstrument.h"

#include "math.h"
#include <QPainter>

QT_BEGIN_NAMESPACE
class QUndoStack;
class ImageArea;
QT_END_NAMESPACE


class AbstractSelection : public AbstractInstrument
{
    Q_OBJECT

public:
    explicit AbstractSelection(QObject *parent = 0);

    void mousePressEvent(QMouseEvent *event, ImageArea &imageArea);
    void mouseMoveEvent(QMouseEvent *event, ImageArea &imageArea);
    void mouseReleaseEvent(QMouseEvent *event, ImageArea &imageArea);
    void clearSelection(ImageArea &imageArea);
    void saveImageChanges(ImageArea &);

    virtual void startSelection(ImageArea &imageArea) = 0;
    virtual void startResizing(ImageArea &imageArea) = 0;
    virtual void startMoving(ImageArea &imageArea) = 0;
    virtual void startAdjusting(ImageArea &imageArea) = 0;
    virtual void select(ImageArea &imageArea) = 0;
    virtual void resize(ImageArea &imageArea) = 0;
    virtual void move(ImageArea &imageArea) = 0;
    virtual void completeSelection(ImageArea &imageArea) = 0;
    virtual void completeResizing(ImageArea &imageArea) = 0;
    virtual void completeMoving(ImageArea &imageArea) = 0;
    virtual void clear() = 0;
    virtual void showMenu(ImageArea &imageArea) = 0;

protected:
    void drawBorder(ImageArea &imageArea);
    void updateCursor(QMouseEvent *event, ImageArea &imageArea);

    QPoint mBottomRightPoint, mTopLeftPoint, mMoveDiffPoint;
    bool mIsPaint, mIsSelectionExists, mIsSelectionMoving, mIsSelectionResizing, mIsImageSelected,
         mIsMouseMoved, mIsSelectionAdjusting;
    int mHeight, mWidth;
    Qt::MouseButton mButton;

};

#endif // ABSTRACTSELECTION_H
