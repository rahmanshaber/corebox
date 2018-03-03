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

#ifndef PROPERTIESW_H
#define PROPERTIESW_H

#include "mymodel.h"

#include <QWidget>


namespace Ui {
class propertiesw;
}

class propertiesw : public QWidget
{
    Q_OBJECT

public:
    explicit propertiesw(QString paths, QWidget *parent = 0);
    ~propertiesw();

    QIcon geticon(const QString &filename);

public slots:
    void checkboxesChanged();
    void numericChanged(QString);

private:
    Ui::propertiesw *ui;
    QString pathName;
    QString permString;
    void permission();
    void general();
    void details();
    QString getDriveInfo(QString path);
};

#endif // PROPERTIESW_H
