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

#ifndef RENAMEDIALOG_H
#define RENAMEDIALOG_H

#include <QDialog>
#include <QPixmap>
#include <QFileInfo>
#include <QShortcut>

#include "corebox/globalfunctions.h"


namespace Ui {
class renameDialog;
}

class renameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit renameDialog(QFileInfo iFile, QWidget *parent = 0);
    ~renameDialog();

private slots:
    void on_done_clicked();

    void on_newName_returnPressed();

private:
    Ui::renameDialog *ui;
    QFileInfo m_iFile;

    void shotcuts();
};

#endif // RENAMEDIALOG_H
