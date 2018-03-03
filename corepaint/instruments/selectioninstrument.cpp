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

#include "selectioninstrument.h"
#include "../imagearea.h"
#include "../undocommand.h"
#include "math.h"

#include <QPainter>
#include <QApplication>
#include <QClipboard>


SelectionInstrument::SelectionInstrument(QObject *parent):AbstractSelection(parent)
{
}

void SelectionInstrument::copyImage(ImageArea &imageArea)
{
    if (mIsSelectionExists)
    {
        imageArea.setImage(mImageCopy);
        QClipboard *globalClipboard = QApplication::clipboard();
        QImage copyImage;
        if(mIsImageSelected)
        {
            copyImage = mSelectedImage;
        }
        else
        {
            copyImage = imageArea.getImage()->copy(mTopLeftPoint.x(), mTopLeftPoint.y(), mWidth, mHeight);
        }
        globalClipboard->setImage(copyImage, QClipboard::Clipboard);
    }
}

void SelectionInstrument::cutImage(ImageArea &imageArea)
{
    if (mIsSelectionExists)
    {
        copyImage(imageArea);
        if(mIsSelectionExists)
        {
            imageArea.setImage(mImageCopy);
            paint(imageArea);
        }
        makeUndoCommand(imageArea);
        if (/*mSelectedImage != mPasteImage || !*/mIsImageSelected)
        {
            imageArea.setImage(mImageCopy);
        }
        else
        {
            clearSelectionBackground(imageArea);
        }
        mTopLeftPoint = QPoint(0, 0);
        mBottomRightPoint = QPoint(0, 0);
        mImageCopy = *imageArea.getImage();
        imageArea.update();
        mIsSelectionExists = false;
        imageArea.restoreCursor();
        emit sendEnableCopyCutActions(false);
    }
}

void SelectionInstrument::pasteImage(ImageArea &imageArea)
{
    QClipboard *globalClipboard = QApplication::clipboard();
    if(mIsSelectionExists)
    {
        imageArea.setImage(mImageCopy);
        paint(imageArea);
        mImageCopy = *imageArea.getImage();
    }
    makeUndoCommand(imageArea);
    mPasteImage = globalClipboard->image();
    if (!mPasteImage.isNull())
    {
        mSelectedImage = mPasteImage;
        mImageCopy = *imageArea.getImage();
        mTopLeftPoint = QPoint(0, 0);
        mBottomRightPoint = QPoint(mPasteImage.width(), mPasteImage.height()) - QPoint(1, 1);
        mHeight = mPasteImage.height();
        mWidth = mPasteImage.width();
        mIsImageSelected = mIsSelectionExists = true;
        paint(imageArea);
        drawBorder(imageArea);
        imageArea.restoreCursor();
        emit sendEnableCopyCutActions(true);
    }
}

void SelectionInstrument::startAdjusting(ImageArea &imageArea)
{
    mImageCopy = *imageArea.getImage();
    mIsImageSelected = false;
}

void SelectionInstrument::startSelection(ImageArea &)
{
}

void SelectionInstrument::startResizing(ImageArea &imageArea)
{
    if (!mIsImageSelected)
    {
        clearSelectionBackground(imageArea);
    }
    if (mIsSelectionAdjusting)
    {
        mIsImageSelected = false;
    }
}

void SelectionInstrument::startMoving(ImageArea &imageArea)
{
    clearSelectionBackground(imageArea);
    if (mIsSelectionAdjusting)
    {
        mIsImageSelected = false;
    }
}

void SelectionInstrument::select(ImageArea &)
{
}

void SelectionInstrument::resize(ImageArea &)
{
}

void SelectionInstrument::move(ImageArea &)
{
}

void SelectionInstrument::completeSelection(ImageArea &imageArea)
{
    mSelectedImage = imageArea.getImage()->copy(mTopLeftPoint.x(),
                                                mTopLeftPoint.y(),
                                                mWidth, mHeight);
    emit sendEnableCopyCutActions(true);
}

void SelectionInstrument::completeResizing(ImageArea &imageArea)
{
    mSelectedImage = imageArea.getImage()->copy(mTopLeftPoint.x(),
                                                mTopLeftPoint.y(),
                                                mWidth, mHeight);
}

void SelectionInstrument::completeMoving(ImageArea &imageArea)
{
    if (mIsSelectionAdjusting)
    {
        mSelectedImage = imageArea.getImage()->copy(mTopLeftPoint.x(),
                                                   mTopLeftPoint.y(),
                                                   mWidth, mHeight);
    }

}

void SelectionInstrument::clearSelectionBackground(ImageArea &imageArea)
{
    if (!mIsSelectionAdjusting)
    {
        QPainter blankPainter(imageArea.getImage());
        blankPainter.setPen(Qt::white);
        blankPainter.setBrush(QBrush(Qt::white));
        blankPainter.setBackgroundMode(Qt::OpaqueMode);
        blankPainter.drawRect(QRect(mTopLeftPoint, mBottomRightPoint - QPoint(1, 1)));
        blankPainter.end();
        mImageCopy = *imageArea.getImage();
    }
}

void SelectionInstrument::clear()
{
    mSelectedImage = QImage();
    emit sendEnableCopyCutActions(false);
}

void SelectionInstrument::paint(ImageArea &imageArea, bool, bool)
{
    if (mIsSelectionExists && !mIsSelectionAdjusting)
    {
        if(mTopLeftPoint != mBottomRightPoint)
        {
            QPainter painter(imageArea.getImage());
            QRect source(0, 0, mSelectedImage.width(), mSelectedImage.height());
            QRect target(mTopLeftPoint, mBottomRightPoint);
            painter.drawImage(target, mSelectedImage, source);
            painter.end();
        }
        imageArea.setEdited(true);
        imageArea.update();
    }
}

void SelectionInstrument::showMenu(ImageArea &)
{
}
