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

#include "textinstrument.h"
#include "../imagearea.h"
#include "../datasingleton.h"
#include "../undocommand.h"
#include "../dialogs/textdialog.h"

#include <QPainter>


TextInstrument::TextInstrument(QObject *parent):AbstractSelection(parent)
{
    mText = QString();
    mIsEdited = false;
}

void TextInstrument::updateText(ImageArea *imageArea, QString textString)
{
    mText = textString;
    imageArea->setImage(mImageCopy);
    if (!mIsEdited)
    {
        makeUndoCommand(*imageArea);
        mIsEdited = true;
    }
    paint(*imageArea);
    drawBorder(*imageArea);
}

void TextInstrument::startAdjusting(ImageArea &)
{
}

void TextInstrument::startSelection(ImageArea &)
{
}

void TextInstrument::startResizing(ImageArea &)
{
}

void TextInstrument::startMoving(ImageArea &)
{
}

void TextInstrument::select(ImageArea &)
{
}

void TextInstrument::resize(ImageArea &imageArea)
{
    paint(imageArea);
}

void TextInstrument::move(ImageArea &imageArea)
{
    paint(imageArea);
}

void TextInstrument::completeSelection(ImageArea &imageArea)
{
    TextDialog *td = new TextDialog(mText, &imageArea);
    connect(td, SIGNAL(textChanged(ImageArea *, QString)), this, SLOT(updateText(ImageArea *, QString)));
    connect(this, SIGNAL(sendCloseTextDialog()), td, SLOT(accept()));
    connect(td, SIGNAL(canceled(ImageArea *)), this, SLOT(cancel(ImageArea *)));
    td->setAttribute(Qt::WA_DeleteOnClose);
    td->show();
}

void TextInstrument::completeResizing(ImageArea &)
{
}

void TextInstrument::completeMoving(ImageArea &)
{
}

void TextInstrument::clear()
{
    mText = QString();
    mIsEdited = false;
    emit sendCloseTextDialog();
}

void TextInstrument::cancel(ImageArea *imageArea)
{
    mText = QString();
    clearSelection(*imageArea);
}

void TextInstrument::paint(ImageArea &imageArea, bool, bool)
{
    if(mTopLeftPoint != mBottomRightPoint)
    {
        QPainter painter(imageArea.getImage());
        painter.setPen(QPen(DataSingleton::Instance()->getPrimaryColor()));
        painter.setFont(DataSingleton::Instance()->getTextFont());
        painter.drawText(QRect(mTopLeftPoint, mBottomRightPoint), mText);
        painter.end();
        imageArea.setEdited(true);
        imageArea.update();
    }
}

void TextInstrument::showMenu(ImageArea &imageArea)
{
    emit sendCloseTextDialog();
    completeSelection(imageArea); //show text dialog
}
