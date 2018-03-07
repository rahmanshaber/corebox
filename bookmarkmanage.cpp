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

#include "bookmarkmanage.h"

#include <QSettings>
#include <QSet>
#include <QList>
#include <QTextStream>
#include <QLineEdit>


void BookmarkManage::checkBookPath()
{
    if (!QDir(cbookPath).exists()) {
        QDir::home().mkdir(".box");
    }
}

void BookmarkManage::createBook()
{
    checkBookPath();
    QFile file(cbookFullPath);
    file.open(QFile::ReadWrite);
    file.close();
}

void BookmarkManage::checkBook()
{
    QFile file(cbookFullPath);
    if (!file.exists()) {
        createBook();
    }
    file.open(QFile::ReadOnly);
    QString s = file.readAll();
    file.close();
    if (s.count() == 0) {
        QTextStream out(&file);
        file.open(QFile::WriteOnly);
        out << QString("[Speed%20Dial]\ncount=0");
        file.close();
    }
}

QStringList BookmarkManage::getBookSections()
{
    QSettings settings(cbookFullPath, QSettings::IniFormat);
    return settings.childGroups();
}

QStringList BookmarkManage::getBookNames(QString sectionName)
{
    QSettings settings(cbookFullPath, QSettings::IniFormat);

    settings.beginGroup(sectionName);
    QStringList list = settings.allKeys();
    settings.endGroup();
    list.removeOne("count");
    return list;
}

bool BookmarkManage::addSection(QString sectionName)
{
    QSettings settings(cbookFullPath, QSettings::IniFormat);

    if (!getBookSections().contains(sectionName, Qt::CaseInsensitive)) {
        settings.beginGroup(sectionName);
        for (int i = 0; i < getBookSections().count(); ++i) {
            if (sectionName.contains(getBookSections().at(i), Qt::CaseInsensitive) == false) {
                settings.setValue("count", int(0));
            }
        }
        settings.endGroup();
        return true;
    }
    return false;
}

bool BookmarkManage::addBookmark(QString sectionName, QString bookmarkName, QString bookPath, QString iconPath)
{
    QSettings settings(cbookFullPath, QSettings::IniFormat);

    if (!getBookNames(sectionName).contains(bookmarkName, Qt::CaseInsensitive)) {
        settings.beginGroup(sectionName);
        for (int i = 0; i < getBookNames(sectionName).count(); ++i) {
            if (bookmarkName.contains("count", Qt::CaseInsensitive) == true) {
                return false;
            } else {
                if (bookmarkName.contains(getBookNames(sectionName).at(i), Qt::CaseInsensitive) == false) {
                    int count = settings.value("count").toInt();
                    settings.setValue("count", count + 1);
                }
            }
        }

        settings.setValue(bookmarkName, bookPath + "$$$" + iconPath);
        settings.endGroup();
        return true;
    }
    return false;
}

void BookmarkManage::delSection(QString sectionName)
{
    QSettings settings(cbookFullPath, QSettings::IniFormat);
    settings.remove(sectionName);
}

void BookmarkManage::delBookmark(QString bookmarkName)
{
    QSettings settings(cbookFullPath, QSettings::IniFormat);
    settings.remove(bookmarkName);
}
void BookmarkManage::delbookmark(QString bookmarkName , QString section)
{
    QSettings settings(cbookFullPath, QSettings::IniFormat);
    settings.beginGroup(section);
    settings.remove(bookmarkName);
    settings.endGroup();
}

void BookmarkManage::editbookmark(QString sectionName, QString bookmarkName, QString bookPath, QString iconPath)
{
    addBookmark(sectionName, bookmarkName, bookPath, iconPath);
}

void BookmarkManage::changeAll(QString oldSectionName, QString oldBookmarkName, QString sectionName, QString bookmarkName, QString bookmarkValue)
{
    QSettings settings(cbookFullPath, QSettings::IniFormat);
    settings.beginGroup(oldSectionName);
    settings.remove(oldBookmarkName);
    settings.setValue("count", settings.childKeys().count());
    settings.endGroup();

    settings.beginGroup(sectionName);
    settings.setValue(bookmarkName, bookmarkValue);
    settings.setValue("count", settings.childKeys().count());
    settings.endGroup();
}

void BookmarkManage::changeSection(QString oldSectionName, QString sectionName, QString bookmarkName, QString bookmarkValue)
{
    QSettings settings(cbookFullPath, QSettings::IniFormat);

    settings.beginGroup(oldSectionName);
    settings.remove(bookmarkName);
    settings.setValue("count", settings.childKeys().count());
    settings.endGroup();

    settings.beginGroup(sectionName);
    settings.setValue(bookmarkName, bookmarkValue);
    settings.setValue("count", settings.childKeys().count());
    settings.endGroup();
}

void BookmarkManage::changeBookmark(QString oldBookmarkName, QString sectionName, QString bookmarkName, QString bookmarkValue)
{
    QSettings settings(cbookFullPath, QSettings::IniFormat);
    settings.remove(oldBookmarkName);

    settings.beginGroup(sectionName);
    settings.setValue(bookmarkName, bookmarkValue);
    settings.endGroup();
}

QString BookmarkManage::bookmarkValues(QString sectionName, QString bookmarkName)
{
    QSettings settings(cbookFullPath, QSettings::IniFormat);

    settings.beginGroup(sectionName);
    return settings.value(bookmarkName).toString();
}

QString BookmarkManage::bookmarkPath(QString sectionName, QString bookmarkName)
{
    QStringList values(bookmarkValues(sectionName, bookmarkName).split("$$$"));
    return values.at(0);
}

QString BookmarkManage::bookmarkIconPath(QString sectionName, QString bookmarkName)
{
    QStringList values(bookmarkValues(sectionName, bookmarkName).split("$$$"));
    return values.at(1);
}

QString BookmarkManage::checkingBookName(QString sectionName, QString bookName)
{
    if (getBookNames(sectionName).contains(bookName, Qt::CaseInsensitive)) {
        return "Bookmark exists.";
    } else {
        return "";
    }
    return "";
}

QString BookmarkManage::keyCount()
{
    return QString("%1").arg(keys().count());
}

QStringList BookmarkManage::keys()
{
    QSettings settings(cbookFullPath, QSettings::IniFormat);
    QStringList list;
    for(int i = 0; i < settings.allKeys().count(); ++i) {
        list.append(QString(settings.allKeys().at(i)).split("/").at(1));
    }
    list = list.toSet().toList();
    //list.removeOne("count");
    return list;
}

QString BookmarkManage::checkingBookPath(QString sectionn, QString bookPath)
{
    QLineEdit *line = new QLineEdit();

    //foreach (QString section, getBookSections()) {
        foreach (QString bName, getBookNames(sectionn)) {
            if (!QString::compare(bookmarkPath(sectionn, bName), bookPath, Qt::CaseSensitive)) {
                line->setText("Path exists in this section.");
                return line->text();
            } else { line->setText(""); continue;}
        }
    //}
    return line->text();
}
