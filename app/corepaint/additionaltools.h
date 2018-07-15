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

#ifndef ADDITIONALTOOLS_H
#define ADDITIONALTOOLS_H

#include <QObject>
#include <QImage>
#include <QImage>
#include <QPainter>
#include <QLabel>
#include <QTransform>
#include <QSize>
#include <QClipboard>
#include <QApplication>

#include "imagearea.h"
#include "dialogs/resizedialog.h"


QT_BEGIN_NAMESPACE
class ImageArea;
class QSize;
QT_END_NAMESPACE


class AdditionalTools : public QObject
{
    Q_OBJECT

public:
    explicit AdditionalTools(ImageArea *pImageArea, QObject *parent);
    ~AdditionalTools();

    void resizeCanvas(int width, int height, bool flag = false);
    void resizeImage();
    void rotateImage(bool flag);
    bool zoomImage(qreal factor);
    
private:
    ImageArea *mPImageArea; /**< A pointer to ImageArea */
    qreal mZoomedFactor; /**< Difference between original and current image */

signals:
    void sendNewImageSize(const QSize&);
    
};

#endif // ADDITIONALTOOLS_H
