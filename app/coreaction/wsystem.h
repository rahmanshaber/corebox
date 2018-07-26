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

#ifndef WSYSTEM_H
#define WSYSTEM_H

#include <QWidget>
#include <QTimer>

#include "dashboard/info_manager.h"


namespace Ui {
class wSystem;
}

class wSystem : public QWidget
{
    Q_OBJECT

public:
    explicit wSystem(QWidget *parent = 0);
    ~wSystem();

private:
    Ui::wSystem *ui;
    InfoManager *info;

    void checkSystem();
};

#endif // WSYSTEM_H
