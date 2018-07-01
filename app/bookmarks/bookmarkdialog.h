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

#ifndef BOOKMARKDIALOG_H
#define BOOKMARKDIALOG_H

#include <QWidget>
#include <QDialog>

#include "bookmarkmanage.h"
#include "corebox/globalfunctions.h"


namespace Ui {
class bookmarkDialog;
}

class bookmarkDialog : public QDialog
{
    Q_OBJECT

public:
    explicit bookmarkDialog(QWidget *parent = 0);
    ~bookmarkDialog();

    void checkPath();

    void setBookIcon(QPixmap pix);
    void setBookPath(const QString &path);
    void setBookName(const QString &bName);
    QString getBookName();
    QString getSectionName();

    bool accepted = false;

private slots:
    void on_done_clicked();
    void bookMarkName_Changed();
    void item_Changed();
    void on_bkSection_currentIndexChanged(const QString &arg1);
    void on_bkName_textChanged(const QString &arg1);

private:
    Ui::bookmarkDialog *ui;
    BookmarkManage bk;
};

#endif // BOOKMARKDIALOG_H
