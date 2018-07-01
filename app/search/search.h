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

#ifndef SEARCH_H
#define SEARCH_H

#include <QIcon>
#include <QMimeDatabase>
#include <QMimeType>
#include <QFileInfo>
#include <QtConcurrent>
#include <QTextStream>
#include <QListWidgetItem>
#include <QTableWidgetItem>
#include <QWidget>
#include <QToolButton>
#include <QProcess>
#include <QShortcut>
#include <QFileDialog>

#include <corebox/globalfunctions.h>


namespace Ui {
class search;
}

class search : public QWidget
{
    Q_OBJECT

public:
    explicit search(QWidget *parent = 0);
    ~search();

    void setPath(const QString &path);

private:
    Ui::search *ui;
    QProcess *cProcess;

    QStringList all;
    QStringList media;
    QStringList image;
    QStringList other;
    QStringList folder;

    void callProcess(bool find);
    void populateItems(const QString &text);
    void toTable(const QStringList &list);
    void shotcuts();
    void startsetup();
    const QStringList &populateByType();

private slots:
    void checkChange(QToolButton *a, QToolButton *b, QToolButton *c, QToolButton *d, QToolButton *e);
    void on_findCMD_clicked();
    void on_locateCMD_clicked();
    void on_typeAll_clicked();
    void on_typePicture_clicked();
    void on_typeFolder_clicked();
    void on_typeMedia_clicked();
    void on_typeother_clicked();
    void on_results_itemDoubleClicked(QTableWidgetItem *item);
    void on_more_clicked(bool checked);
    void on_searchFF_textChanged(const QString &arg1);
    void on_setfolder_clicked();

};

#endif // SEARCH_H
