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

#ifndef COLORCHOOSER_H
#define COLORCHOOSER_H

#include <QLabel>

QT_BEGIN_NAMESPACE
class QColor;
class QPixmap;
class QPainter;
class QMouseEvent;
class QColorDialog;
QT_END_NAMESPACE


class ColorChooser : public QLabel
{
    Q_OBJECT

public:
    explicit ColorChooser(const int &r, const int &g, const int &b,QWidget *parent = 0);
    ~ColorChooser();
    
private:
    QColor *mCurrentColor;
    QPixmap *mPixmapColor;
    QPainter *mPainterColor;

public slots:
    void setColor(const QColor &color);

signals:
    void sendColor(const QColor &);

protected:
    void mousePressEvent(QMouseEvent *event);
    
};

#endif // COLORCHOOSER_H
