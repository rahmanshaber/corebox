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

#include "abstractinstrument.h"
#include "../imagearea.h"
#include "../undocommand.h"


AbstractInstrument::AbstractInstrument(QObject *parent):QObject(parent)
{}

void AbstractInstrument::makeUndoCommand(ImageArea &imageArea)
{
    imageArea.pushUndoCommand(new UndoCommand(imageArea.getImage(), imageArea));
}
