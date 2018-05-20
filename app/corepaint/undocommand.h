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

#ifndef UNDOCOMMAND_H
#define UNDOCOMMAND_H

#include <QUndoCommand>
#include <QImage>

#include "imagearea.h"

class UndoCommand : public QUndoCommand
{
public:
    UndoCommand(const QImage* img, ImageArea &imgArea, QUndoCommand *parent = 0);

    virtual void undo();
    virtual void redo();
private:
    QImage mPrevImage;
    QImage mCurrImage;
    ImageArea& mImageArea;
};

#endif // UNDOCOMMAND_H
