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

#ifndef ABOUT_H
#define ABOUT_H

#include <QWidget>

#include "corebox/globalfunctions.h"


namespace Ui {
class about;
}

class about : public QWidget
{
    Q_OBJECT

public:
    explicit about(QWidget *parent = 0);
    ~about();

private slots:
    void on_corebox_clicked();
    void on_releaseNotes_clicked();
    void on_helpUs_clicked();

private:
    Ui::about *ui;

    void pageClick(QPushButton *btn, int i, QString title);
};

#endif // ABOUT_H
