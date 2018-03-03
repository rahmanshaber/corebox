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

#ifndef APPLICATIONDIALOG_H
#define APPLICATIONDIALOG_H

#include "desktopfile.h"

#include <QDialog>
#include <QTreeWidget>


class ApplicationDialog : public QDialog {
  Q_OBJECT

public:
  explicit ApplicationDialog(QWidget *parent = 0);
  QString getCurrentLauncher() const;

protected slots:
  void updateCommand(QTreeWidgetItem* current, QTreeWidgetItem* previous);

protected:
  QTreeWidget* appList;
  QLineEdit* edtCommand;
  DesktopFile result;
  QIcon defaultIcon;
  QMap<QString, QStringList> catNames;
  QMap<QString, QTreeWidgetItem*> categories;
  QMap<QString, QTreeWidgetItem*> applications;
  QTreeWidgetItem* findCategory(const DesktopFile &app);
  void createCategories();
};

#endif // APPLICATIONDIALOG_H
