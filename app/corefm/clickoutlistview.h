#ifndef CLICKOUTLISTVIEW_H
#define CLICKOUTLISTVIEW_H

#include <QWidget>
#include <QMouseEvent>
#include <QDebug>
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
