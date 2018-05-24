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


#ifndef CLICKOUTLISTVIEW_H
#define CLICKOUTLISTVIEW_H

#include <QWidget>
#include <QMouseEvent>
#include <QListView>
#include <QTreeView>


class ClickOutListview : public QListView
{
    Q_OBJECT
    Q_DISABLE_COPY(ClickOutListview)
public:
    explicit ClickOutListview(QWidget* parent = Q_NULLPTR) :
        QListView(parent){}

Q_SIGNALS:
    void clickedOut();

protected:
    void mousePressEvent(QMouseEvent *event) {
        if (!indexAt(event->pos()).isValid()) {
            clickedOut();
        }
        QListView::mousePressEvent(event);
    }
};

class ClickOutTreeview : public QTreeView
{
    Q_OBJECT
    Q_DISABLE_COPY(ClickOutTreeview)
public:
    explicit ClickOutTreeview(QWidget* parent = Q_NULLPTR) :
        QTreeView(parent){}

Q_SIGNALS:
    void clickedOut();

protected:
    void mousePressEvent(QMouseEvent *event) {
        if (!indexAt(event->pos()).isValid()) {
            clickedOut();
        }
        QTreeView::mousePressEvent(event);
    }
};


#endif // CLICKOUTLISTVIEW_H
