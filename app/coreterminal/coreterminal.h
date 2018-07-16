/*
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

#ifndef CORETERMWIDGET_H
#define CORETERMWIDGET_H

#pragma once

#include "qtermwidget.h"

#include <iostream>
#include <fstream>
#include <QFileSystemWatcher>
#include <QFileInfo>
#include <QProcessEnvironment>
#include <QCloseEvent>
#include <QWidget>
#include <QDebug>
#include <QTabWidget>
#include <QShortcut>

#include "../corebox/corebox.h"


class coreterminal : public QTermWidget
{
    Q_OBJECT

public :
    coreterminal(QWidget *parent = 0);
    coreterminal(const QString &workDir, QWidget *parent = 0);
    coreterminal(const QString &workDir, const QString &command, QWidget *parent = 0);

    QString currentWorkingDirectory();

private:
    QString oldCWD;

private slots:
    void handleFSWSignals(QString);

protected:
    void closeEvent(QCloseEvent *cEvent);

Q_SIGNALS:
    void chDir(QString);

};

static QFileSystemWatcher *watcher = new QFileSystemWatcher();

#endif // CORETERMWIDGET_H
