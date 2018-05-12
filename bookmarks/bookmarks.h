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

#ifndef BOOKMARKS_H
#define BOOKMARKS_H

#include "bookmarkmanage.h"

#include <QWidget>
#include <QDir>
#include <QFile>
#include <QListWidgetItem>
#include <QMimeData>
#include <QMouseEvent>
#include <QTableWidgetItem>


namespace Ui {
class bookmarks;
}

class bookmarks : public QWidget
{
    Q_OBJECT

public:
    explicit bookmarks(QWidget *parent = 0);
    ~bookmarks();

    void saveBookMark(QString section, QString bookmarkName, QString bookmarkPath, QString bookmarkIcon);
    void callBookMarkDialog(QWidget *parent, QString currentPath);

private slots:
    void on_section_itemClicked(QListWidgetItem *item);
    void on_addSection_clicked();
    void on_deleteSection_clicked();
    void on_reload_clicked();
    void on_sectionDone_clicked();
    void on_sectionCancel_clicked();
    void on_sectionName_textChanged(const QString &arg1);
    void on_bookmarkEdit_clicked();
    void on_bookmarkDelete_clicked();
    void on_editCancel_clicked();
    void on_bookmarkName_textChanged(const QString &arg1);
    void on_pathName_textChanged(const QString &arg1);
    void on_boklist_itemDoubleClicked(QTableWidgetItem *item);
    void on_boklist_itemSelectionChanged();
    void on_editDone_clicked();
    void on_selectSection_currentIndexChanged(const QString &arg1);
    void on_searchBook_textChanged(const QString &arg1);
    void on_cTools_clicked();

private:
    Ui::bookmarks *ui;
    BookmarkManage bk;
    QDir currentDir;
    void sectionRefresh();

protected:
    bool eventFilter(QObject *obj, QEvent *event);

};

#endif // BOOKMARKS_H
