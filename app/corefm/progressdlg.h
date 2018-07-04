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

#ifndef PROGRESSDLG_H
#define PROGRESSDLG_H

#include <QtGui>
#include <QDialog>
#include <QLabel>
#include <QProgressBar>
#include <QtGui>
#include <QPushButton>


class myProgressDialog : public QDialog
{
    Q_OBJECT

public:
    myProgressDialog(QString title);

public slots:
    void setShowing();
    void update(qint64 bytes, qint64 total, QString name);

private:
    QLabel *filename;
    QLabel *transferInfo;
    QProgressBar *bar;
    QPushButton *button;

    QTime  *remainingTimer;

    qint64 runningTotal;
    int oldSeconds;

};


#endif // PROGRESSDLG_H
