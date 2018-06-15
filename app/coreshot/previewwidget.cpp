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

#include "previewwidget.h"

#include <QDrag>
#include <QTemporaryDir>
#include <QTemporaryFile>
#include <QFileInfo>
#include <QStandardPaths>


PreviewWidget::PreviewWidget(QWidget *parent) : QLabel(parent)
{
    cMimeData = new QMimeData;
    setAlignment(Qt::AlignCenter);
    setFocus();
}

PreviewWidget::~PreviewWidget()
{
    delete cMimeData;
}

void PreviewWidget::resizeEvent(QResizeEvent * event)
{
    Q_UNUSED(event);
    if (cOriginalPixmap.isNull())
        return;

    QPixmap test = cOriginalPixmap.scaled(size(), Qt::KeepAspectRatio);

    setPixmap(test);
}

void PreviewWidget::setOriginalPixmap(const QPixmap &pix)
{
    cOriginalPixmap = pix;
    QPixmap test = cOriginalPixmap.scaled(size(), Qt::KeepAspectRatio,Qt::SmoothTransformation);
    setPixmap(test);
}

const QPixmap &PreviewWidget::originalPixmap() const
{
    return cOriginalPixmap;
}

