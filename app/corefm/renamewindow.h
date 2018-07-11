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

#ifndef RENAMEWINDOW_H
#define RENAMEWINDOW_H

#include <QPixmap>
#include <QWidget>
#include <QFileInfo>

#include "corebox/globalfunctions.h"


namespace Ui {
class RenameWindow;
}

class RenameWindow : public QWidget
{
    Q_OBJECT

public:
    explicit RenameWindow(QFileInfo iFile, QWidget *parent = 0);
    ~RenameWindow();

private slots:
    void on_ok_clicked();

private:
    Ui::RenameWindow *ui;
    QFileInfo m_iFile;
};

#endif // RENAMEWINDOW_H
