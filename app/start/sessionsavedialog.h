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

#ifndef SESSIONSAVEDIALOG_H
#define SESSIONSAVEDIALOG_H

#include <QDialog>
#include <QSettings>
#include <QDir>
#include <QDebug>


namespace Ui {
class sessionSaveDialog;
}

class sessionSaveDialog : public QDialog
{
    Q_OBJECT

public:
    explicit sessionSaveDialog(QWidget *parent = 0);
    ~sessionSaveDialog();
    QString sName;

signals:
    void nameOk();

private slots:
    void on_ok_clicked();
    void on_sessionName_textChanged(const QString &arg1);

private:
    Ui::sessionSaveDialog *ui;
    QStringList nameList;
};

#endif // SESSIONSAVEDIALOG_H
