/*
CoreTerminal is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; version 2
of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, see {http://www.gnu.org/licenses/}. */

#ifndef CORETERMINAL_H
#define CORETERMINAL_H

#include <QWidget>

#include "qtermwidget.h"


class coreterminal : public QWidget
{
    Q_OBJECT

public:
    explicit coreterminal(QWidget *parent = 0);
    ~coreterminal();
};

#endif // CORETERMINAL_H
