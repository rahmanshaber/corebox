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

#ifndef SELECTIONINSTRUMENT_H
#define SELECTIONINSTRUMENT_H

#include "abstractselection.h"

QT_BEGIN_NAMESPACE
class QUndoStack;
QT_END_NAMESPACE

class SelectionInstrument : public AbstractSelection
{
    Q_OBJECT

public:
    explicit SelectionInstrument(QObject *parent = 0);

    void clearSelectionBackground(ImageArea &imageArea);
    void copyImage(ImageArea &imageArea);
    void pasteImage(ImageArea &imageArea);
    void cutImage(ImageArea &imageArea);

private:
    void startAdjusting(ImageArea &imageArea);
    void startSelection(ImageArea &);
    void startResizing(ImageArea &imageArea);
    void startMoving(ImageArea &imageArea);
    void select(ImageArea &);
    void resize(ImageArea &);
    void move(ImageArea &);
    void completeSelection(ImageArea &imageArea);
    void completeResizing(ImageArea &imageArea);
    void completeMoving(ImageArea &imageArea);
    void clear();
    void paint(ImageArea &imageArea, bool = false, bool = false);
    void showMenu(ImageArea &);

    QImage mSelectedImage,  // Copy of selected image.
           mPasteImage;     //Image to paste

signals:
    void sendEnableCopyCutActions(bool enable);
    void sendEnableSelectionInstrument(bool enable);

};

#endif // SELECTIONINSTRUMENT_H
