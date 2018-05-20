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

#ifndef BOOKMARKMANAGE_H
#define BOOKMARKMANAGE_H

#include <QWidget>
#include <QSettings>
#include <QFile>
#include <QDir>


class BookmarkManage
{

public:
    void checkBook();
    QStringList getBookSections();
    QStringList getBookNames(QString sectionName);
    bool addSection(QString sectionName);
    bool addBookmark(QString sectionName, QString bookmarkName, QString bookPath);

    QString bookmarkPath(QString sectionName, QString bookmarkName);
    QString checkingBookName(QString sectionName, QString bookName);
    QString checkingBookPath(QString section, QString bookPath);
    QString checkingBookPathEx(QString bookPath);
    QString cbookName = "CoreBoxBook";
    QString cbookPath = QDir::homePath() + "/.config/coreBox";
    QString cbookFullPath = cbookPath + "/" + cbookName;

    void delSection(QString sectionName);
    void delBookmark(QString bookmarkName);
    void changeAll(QString oldSectionName, QString oldBookmarkName, QString sectionName, QString bookmarkName, QString bookmarkValue);
    void editbookmark(QString sectionName, QString bookmarkName, QString bookPath);
    void delbookmark(QString bookmarkName, QString section);

private:
    void createBook();
    void checkBookPath();
    void changeSection(QString oldSectionName, QString sectionName, QString bookmarkName, QString bookmarkValue);
    void changeBookmark(QString oldBookmarkName, QString sectionName, QString bookmarkName, QString bookmarkValue);

    QStringList keys();
    QString bookmarkValues(QString sectionName, QString bookmarkName);
    QString keyCount();

};

#endif // BOOKMARKMANAGE_H

