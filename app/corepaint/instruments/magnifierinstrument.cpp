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

#include "magnifierinstrument.h"
#include "../imagearea.h"


MagnifierInstrument::MagnifierInstrument(QObject *parent) :AbstractInstrument(parent)
{
}

void MagnifierInstrument::mousePressEvent(QMouseEvent *event, ImageArea &imageArea)
{
    if(event->button() == Qt::LeftButton || event->button() == Qt::RightButton)
    {
        imageArea.setIsPaint(true);
    }
}

void MagnifierInstrument::mouseMoveEvent(QMouseEvent *, ImageArea &)
{

}

void MagnifierInstrument::mouseReleaseEvent(QMouseEvent *event, ImageArea &imageArea)
{
    if(imageArea.isPaint())
    {
        if(event->button() == Qt::LeftButton)
        {
            if(imageArea.zoomImage(2.0))
            {
                imageArea.setZoomFactor(2.0);
            }
        }
        else if(event->button() == Qt::RightButton)
        {
            if(imageArea.zoomImage(0.5))
            {
                imageArea.setZoomFactor(0.5);
            }
        }
        imageArea.setIsPaint(false);
    }
}

void MagnifierInstrument::paint(ImageArea &, bool, bool)
{

}
