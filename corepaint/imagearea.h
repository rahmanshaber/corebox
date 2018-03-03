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

#ifndef IMAGEAREA_H
#define IMAGEAREA_H

#include "easypaintenums.h"
#include "additionaltools.h"

#include <QWidget>
#include <QImage>
#include <QPixmap>

QT_BEGIN_NAMESPACE
class QUndoStack;
QT_END_NAMESPACE

class UndoCommand;
class AbstractInstrument;
class AbstractEffect;


class ImageArea : public QWidget
{
    Q_OBJECT

public:
    explicit ImageArea(const bool &isOpen, const QString &filePath, QWidget *parent);
    ~ImageArea();

    bool save();
    bool saveAs();
    void resizeImage();
    void resizeCanvas();
    void rotateImage(bool flag);

    inline QString getFileName() { return (mFilePath.isEmpty() ? mFilePath :
                                           mFilePath.split('/').last()); }
    inline QImage* getImage() { return mImage; }
    inline void setImage(const QImage &image) { *mImage = image; }
    inline void setEdited(bool flag) { mIsEdited = flag; }
    inline bool getEdited() { return mIsEdited; }

    void restoreCursor();
    bool zoomImage(qreal factor);
    inline void setZoomFactor(qreal factor) { mZoomFactor *= factor; }
    inline qreal getZoomFactor() { return mZoomFactor; }
    inline QUndoStack* getUndoStack() { return mUndoStack; }
    inline void setIsPaint(bool isPaint) { mIsPaint = isPaint; }
    inline bool isPaint() { return mIsPaint; }
    inline void emitPrimaryColorView() { emit sendPrimaryColorView(); }
    inline void emitSecondaryColorView() { emit sendSecondaryColorView(); }
    inline void emitColor(QColor &color) { emit sendColor(color); }
    inline void emitRestorePreviousInstrument() { emit sendRestorePreviousInstrument(); }

    void copyImage();
    void pasteImage();
    void cutImage();
    void saveImageChanges();
    void clearSelection();
    void pushUndoCommand(UndoCommand *command);

    QString mFilePath; /**< Path where located image. */
private:
    void initializeImage();
    void open();
    void open(const QString &filePath);
    void drawCursor();
    void makeFormatsFilters();

    QImage *mImage,  /**< Main image. */
           mImageCopy; /**< Copy of main image, need for events. */ // ?????????????
    AdditionalTools *mAdditionalTools;
    QString mOpenFilter; /**< Supported open formats filter. */
    QString mSaveFilter; /**< Supported save formats filter. */
    bool mIsEdited, mIsPaint, mIsResize, mRightButtonPressed;
    QPixmap *mPixmap;
    QCursor *mCurrentCursor;
    qreal mZoomFactor;
    QUndoStack *mUndoStack;
    QVector<AbstractInstrument*> mInstrumentsHandlers;
    AbstractInstrument *mInstrumentHandler;


signals:
    void sendPrimaryColorView();
    void sendSecondaryColorView();
    void sendNewImageSize(const QSize&);
    void sendCursorPos(const QPoint&);
    void sendColor(const QColor&);
    void sendRestorePreviousInstrument();
    void sendSetInstrument(InstrumentsEnum);
    void sendEnableCopyCutActions(bool enable);
    void sendEnableSelectionInstrument(bool enable);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    
};

#endif // IMAGEAREA_H
