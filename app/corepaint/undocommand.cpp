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

#include "undocommand.h"


UndoCommand::UndoCommand(const QImage *img, ImageArea &imgArea, QUndoCommand *parent)
    : QUndoCommand(parent), mPrevImage(*img), mImageArea(imgArea)
{
    mCurrImage = mPrevImage;
}

void UndoCommand::undo()
{
    mImageArea.clearSelection();
    mCurrImage = *(mImageArea.getImage());
    mImageArea.setImage(mPrevImage);
    mImageArea.update();
    mImageArea.saveImageChanges();
}

void UndoCommand::redo()
{
    mImageArea.setImage(mCurrImage);
    mImageArea.update();
    mImageArea.saveImageChanges();
}
