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

#ifndef PREVIEWWIDGET_H
#define PREVIEWWIDGET_H

#include <QLabel>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QDrag>
#include <QMimeData>
#include <QTemporaryDir>
#include <QTemporaryFile>
#include <QFileInfo>
#include <QStandardPaths>


class PreviewWidget : public QLabel
{
    Q_OBJECT

public:
    explicit PreviewWidget(QWidget *parent = 0);
    ~PreviewWidget();
    void resizeEvent(QResizeEvent *);
    void setOriginalPixmap(const QPixmap& pix);
    const QPixmap& originalPixmap() const;

private:
    QPixmap cOriginalPixmap;
    QDrag *cDrag;
    QMimeData * cMimeData;

};

#endif // PREVIEWWIDGET_H
