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

#include "colorpickerinstrument.h"


ColorpickerInstrument::ColorpickerInstrument(QObject *parent) :AbstractInstrument(parent)
{
}

void ColorpickerInstrument::mousePressEvent(QMouseEvent *event, ImageArea &imageArea)
{
    if(event->button() == Qt::LeftButton || event->button() == Qt::RightButton)
    {
        imageArea.setIsPaint(true);
    }
}

void ColorpickerInstrument::mouseMoveEvent(QMouseEvent *event, ImageArea &imageArea)
{
    QRgb pixel(imageArea.getImage()->pixel(event->pos()));
    QColor getColor(pixel);
    imageArea.emitColor(getColor);
}

void ColorpickerInstrument::mouseReleaseEvent(QMouseEvent *event, ImageArea &imageArea)
{
    if(imageArea.isPaint())
    {
        mStartPoint = mEndPoint = event->pos();
        if(event->button() == Qt::LeftButton)
        {
            paint(imageArea, false);
            imageArea.emitPrimaryColorView();
        }
        else if(event->button() == Qt::RightButton)
        {
            paint(imageArea, true);
            imageArea.emitSecondaryColorView();
        }
        imageArea.setIsPaint(false);
        imageArea.emitRestorePreviousInstrument();
    }
}

void ColorpickerInstrument::paint(ImageArea &imageArea, bool isSecondaryColor, bool)
{
    bool inArea(true);
    if(mStartPoint.x() < 0 || mStartPoint.y() < 0
            || mStartPoint.x() > imageArea.getImage()->width()
            || mStartPoint.y() > imageArea.getImage()->height())
        inArea = false;

    if(inArea && isSecondaryColor)
    {
        QRgb pixel(imageArea.getImage()->pixel(mStartPoint));
        QColor getColor(pixel);
        DataSingleton::Instance()->setSecondaryColor(getColor);
    }
    if(inArea)
    {
        QRgb pixel(imageArea.getImage()->pixel(mStartPoint));
        QColor getColor(pixel);
        DataSingleton::Instance()->setPrimaryColor(getColor);
    }
}
