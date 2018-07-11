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

#ifndef COREPAD_H
#define COREPAD_H

#include <QFile>
#include <QFileDialog>
#include <QShortcut>
#include <QFileInfo>
#include <QDate>
#include <QDateTime>
#include <QMessageBox>
#include <QDebug>
#include <QWidget>
#include <QCloseEvent>

#include "corebox/corebox.h"
#include "corebox/globalfunctions.h"
#include "bookmarks/bookmarks.h"
#include "coreedit.h"


namespace Ui {
class corepad;
}

class corepad : public QWidget
{
    Q_OBJECT

public:
    explicit corepad(QWidget *parent = 0);
    ~corepad();

    QString workFilePath;
    void openText(const QString &filePath);
    bool initializeNewTab(const QString &filePath);
    int tabsCount();
    QString currentFilePath(int index);

private slots:
    void textCopyAvailable(bool b);
    void textUndoAvailable(bool b);
    void textRedoAvailable(bool b);
    void textTextChanged();

    void on_notes_currentChanged(int index);
    void on_notes_tabCloseRequested(int index);

    void on_cOpen_clicked();
    void on_cNew_clicked();
    bool on_cSave_clicked();
    void on_cSaveAs_clicked();
    void on_cCopy_clicked();
    void on_cPaste_clicked();
    void on_cCut_clicked();
    void on_cUndo_clicked();
    void on_cRedo_clicked();
    void on_addDate_clicked();
    void on_bookMarkIt_clicked();
    void on_fontSize_currentIndexChanged(const QString &arg1);
    void on_search_clicked(bool checked);
    void on_searchHere_textChanged(const QString &arg1);
    void on_nextW_clicked();
    void on_previousW_clicked();
    void quitClicked();
    void on_fontShow_clicked();

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::corepad *ui;
    coreedit *text;
    QStringList tabInfo; //Store tab information (Index, IsSaved, IsUpdated, FilePath)

    bool started;
    bool saveTo(const QString &filePath);
    void findS(QString searchS, bool reverse, QTextDocument::FindFlags flag = 0);
    bool closeTab(int index);

    //Accessing information through tab information

    QString isCurrentSaved(int index);
    QString isCurrentUpdated(int index);

    //Saving information at tab information
    bool setCurrent(int index, int isSaved, int isUpdated, const QString &filePath);

    void shotcuts();
    void reIndex();
};

#endif // COREPAD_H
