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

#ifndef EASYPAINTENUMS_H
#define EASYPAINTENUMS_H

typedef enum
{
    NONE_INSTRUMENT = 0,
    CURSOR,
    ERASER,
    PEN,
    LINE,
    COLORPICKER,
    SPRAY,
    FILL,
    RECTANGLE,
    ELLIPSE,
    CURVELINE,
    TEXT,

    // Don't use it. (Used to know count of current instrument)
    INSTRUMENTS_COUNT
} InstrumentsEnum;

#endif // EASYPAINTENUMS_H
