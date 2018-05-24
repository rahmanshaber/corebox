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

#include "desktopfile.h"

#include <QFile>

#include "properties.h"


/**
 * @brief Loads desktop file
 * @param fileName
 */
DesktopFile::DesktopFile(const QString &fileName) {

  // Store file name
  this->fileName = fileName;

  // File validity
  if (!QFile::exists(fileName)) {
    return;
  }

  // Loads .desktop file (read from 'Desktop Entry' group)
  Properties desktop(fileName, "Desktop Entry");
  name = desktop.value("Name", "").toString();
  exec = desktop.value("Exec", "").toString();
  icon = desktop.value("Icon", "").toString();
  type = desktop.value("Type", "Application").toString();
  categories = desktop.value("Categories").toString().remove(" ").split(";");
  mimeType = desktop.value("MimeType").toString().remove(" ").split(";");

  // Fix categories
  if (categories.first().compare("") == 0) {
    categories.removeFirst();
  }

  if(fileName.endsWith("vlc.desktop")){exec = "vlc";};
}

QString DesktopFile::getFileName() const {
  return fileName;
}

QString DesktopFile::getPureFileName() const {
  return fileName.split("/").last().remove(".desktop");
}

QString DesktopFile::getName() const {
  return name;
}

QString DesktopFile::getExec() const {
  return exec;
}

QString DesktopFile::getIcon() const {
  return icon;
}

QString DesktopFile::getType() const {
  return type;
}

QStringList DesktopFile::getCategories() const {
  return categories;
}

QStringList DesktopFile::getMimeType() const {
  return mimeType;
}
