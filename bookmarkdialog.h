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

#include "bookmarkmanage.h"
#include "globalfunctions.h"

#include <QDialog>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QShortcut>


class bookmarkDialog : public QDialog
{
    Q_OBJECT

public:
    explicit bookmarkDialog(QWidget *parent = 0);
    ~bookmarkDialog();

    void checkPath();

    bool accepted = false;
    bool canceled;

    QPushButton *accept;
    QPushButton *cancel;

    QLabel *statusLabel;
    QLabel *lSection;
    QLabel *lbkName;
    QLabel *lpath;
    QLabel *iconLabel;
    QLabel *pathLabel;

    QLineEdit *bookMarkName;
    QComboBox *sectionName;

private slots:
    void ok_clicked();
    void bookMarkName_Changed();
    void item_Changed();

private:
    BookmarkManage bk;
};

#endif // BOOKMARKDIALOG_H
