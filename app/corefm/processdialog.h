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

#ifndef PROCESSDIALOG_H
#define PROCESSDIALOG_H

#include <QDialog>
#include <QProcess>
#include <QListWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>


/**
 * @class ProcessDialog
 * @brief Represents dialog which is shown during custom action
 * @author Michal Rost
 * @date 30.12.2012
 */
class ProcessDialog : public QDialog {
  Q_OBJECT

public:
  explicit ProcessDialog(QProcess* proc, const QString &procName = "",QWidget *parent = 0);

protected slots:
  void onProcFinished();
  void onProcStarted();
  void onProcStdOut();

protected:
  QProcess* procPtr;
  QListWidget* output;

};

#endif // PROCESSDIALOG_H
