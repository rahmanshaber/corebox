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

#include <QSet>
#include <QList>
#include <QTextStream>
#include <QLineEdit>


void BookmarkManage::checkBookPath()
{
    if (!QDir(cbookPath).exists()) {
        QDir::home().mkdir(".config/coreBox");
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
    QSettings bkGet(cbookFullPath, QSettings::IniFormat);
    return bkGet.childGroups();
}

QStringList BookmarkManage::getBookNames(QString sectionName)
{
    QSettings bkGet(cbookFullPath, QSettings::IniFormat);

    bkGet.beginGroup(sectionName);
    QStringList list = bkGet.allKeys();
    bkGet.endGroup();
    list.removeOne("count");
    return list;
}

bool BookmarkManage::addSection(QString sectionName)
{
    QSettings bkGet(cbookFullPath, QSettings::IniFormat);

    if (!getBookSections().contains(sectionName, Qt::CaseInsensitive)) {
        bkGet.beginGroup(sectionName);
        for (int i = 0; i < getBookSections().count(); ++i) {
            if (sectionName.contains(getBookSections().at(i), Qt::CaseInsensitive) == false) {
                bkGet.setValue("count", int(0));
            }
        }
        bkGet.endGroup();
        return true;
    }
    return false;
}

bool BookmarkManage::addBookmark(QString sectionName, QString bookmarkName, QString bookPath)
{
    QSettings bkGet(cbookFullPath, QSettings::IniFormat);

    if (!getBookNames(sectionName).contains(bookmarkName, Qt::CaseInsensitive)) {
        bkGet.beginGroup(sectionName);
        for (int i = 0; i < getBookNames(sectionName).count(); ++i) {
            if (bookmarkName.contains("count", Qt::CaseInsensitive) == true) {
                return false;
            } else {
                if (bookmarkName.contains(getBookNames(sectionName).at(i), Qt::CaseInsensitive) == false) {
                    int count = bkGet.value("count").toInt();
                    bkGet.setValue("count", count + 1);
                }
            }
        }

        bkGet.setValue(bookmarkName, bookPath);
        bkGet.endGroup();
        return true;
    }
    return false;
}

void BookmarkManage::delSection(QString sectionName)
{
    QSettings bkGet(cbookFullPath, QSettings::IniFormat);
    bkGet.remove(sectionName);
}

void BookmarkManage::delBookmark(QString bookmarkName)
{
    QSettings bkGet(cbookFullPath, QSettings::IniFormat);
    bkGet.remove(bookmarkName);
}
void BookmarkManage::delbookmark(QString bookmarkName , QString section)
{
    QSettings bkGet(cbookFullPath, QSettings::IniFormat);
    bkGet.beginGroup(section);
    bkGet.remove(bookmarkName);
    bkGet.endGroup();
}

void BookmarkManage::editbookmark(QString sectionName, QString bookmarkName, QString bookPath)
{
    addBookmark(sectionName, bookmarkName, bookPath);
}

void BookmarkManage::changeAll(QString oldSectionName, QString oldBookmarkName, QString sectionName, QString bookmarkName, QString bookmarkValue)
{
    QSettings bkGet(cbookFullPath, QSettings::IniFormat);
    bkGet.beginGroup(oldSectionName);
    bkGet.remove(oldBookmarkName);
    bkGet.setValue("count", bkGet.childKeys().count());
    bkGet.endGroup();

    bkGet.beginGroup(sectionName);
    bkGet.setValue(bookmarkName, bookmarkValue);
    bkGet.setValue("count", bkGet.childKeys().count());
    bkGet.endGroup();
}

void BookmarkManage::changeSection(QString oldSectionName, QString sectionName, QString bookmarkName, QString bookmarkValue)
{
    QSettings bkGet(cbookFullPath, QSettings::IniFormat);

    bkGet.beginGroup(oldSectionName);
    bkGet.remove(bookmarkName);
    bkGet.setValue("count", bkGet.childKeys().count());
    bkGet.endGroup();

    bkGet.beginGroup(sectionName);
    bkGet.setValue(bookmarkName, bookmarkValue);
    bkGet.setValue("count", bkGet.childKeys().count());
    bkGet.endGroup();
}

void BookmarkManage::changeBookmark(QString oldBookmarkName, QString sectionName, QString bookmarkName, QString bookmarkValue)
{
    QSettings bkGet(cbookFullPath, QSettings::IniFormat);
    bkGet.remove(oldBookmarkName);

    bkGet.beginGroup(sectionName);
    bkGet.setValue(bookmarkName, bookmarkValue);
    bkGet.endGroup();
}

QString BookmarkManage::bookmarkValues(QString sectionName, QString bookmarkName)
{
    QSettings bkGet(cbookFullPath, QSettings::IniFormat);

    bkGet.beginGroup(sectionName);
    return bkGet.value(bookmarkName).toString();
}

QString BookmarkManage::bookmarkPath(QString sectionName, QString bookmarkName)
{
    //QStringList values(bookmarkValues(sectionName, bookmarkName).split("$$$"));
    //return values.at(0);
    return bookmarkValues(sectionName, bookmarkName);
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
    QSettings bkGet(cbookFullPath, QSettings::IniFormat);
    QStringList list;
    for(int i = 0; i < bkGet.allKeys().count(); ++i) {
        list.append(QString(bkGet.allKeys().at(i)).split("/").at(1));
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

QString BookmarkManage::checkingBookPathEx(QString bookPath) {
    QLineEdit *line = new QLineEdit();

    foreach (QString section, getBookSections()) {
        foreach (QString bName, getBookNames(section)) {
            if (!QString::compare(bookmarkPath(section, bName), bookPath, Qt::CaseSensitive)) {
                line->setText(QString("\"%1\" exists in \"%2\" section.").arg(bookPath).arg(section));
                return line->text();
            } else { line->setText(""); continue;}
        }
    }
    return line->text();
}
