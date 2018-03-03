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

#ifndef ICONDLG_H
#define ICONDLG_H

#include <QtGui>
#include <QDialog>
#include <QListWidget>
#include <QVBoxLayout>
#include <QCompleter>
#include <QDialogButtonBox>


class icondlg : public QDialog
{

Q_OBJECT

public:
    icondlg();
    QString result;

public slots:
    void scanTheme();
    void loadIcons();
    void accept();

private:
    QListWidget *iconList;
    QDialogButtonBox *buttons;
    QVBoxLayout *layout;
    QStringList fileNames;
    QStringList themes;
    QFutureWatcher<void> thread;
};

#endif // ICONDLG_H
