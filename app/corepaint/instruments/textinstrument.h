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

#ifndef TEXTINSTRUMENT_H
#define TEXTINSTRUMENT_H

#include "abstractselection.h"
#include "../imagearea.h"
#include "../datasingleton.h"
#include "../undocommand.h"
#include "../dialogs/textdialog.h"

#include <QObject>
#include <QPainter>


class TextInstrument : public AbstractSelection
{
    Q_OBJECT
public:
    explicit TextInstrument(QObject *parent = 0);

private:
    void startAdjusting(ImageArea &);
    void startSelection(ImageArea &);
    void startResizing(ImageArea &);
    void startMoving(ImageArea &);
    void select(ImageArea &);
    void resize(ImageArea &imageArea);
    void move(ImageArea &imageArea);
    void completeSelection(ImageArea &imageArea);
    void completeResizing(ImageArea &);
    void completeMoving(ImageArea &);
    void clear();
    void paint(ImageArea &imageArea, bool = false, bool = false);
    void showMenu(ImageArea &imageArea);

    QString mText;
    bool mIsEdited;

signals:
    void sendCloseTextDialog();

private slots:
    void updateText(ImageArea *, QString);
    void cancel(ImageArea *);

};

#endif // TEXTINSTRUMENT_H
