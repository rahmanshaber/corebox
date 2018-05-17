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

#include <QWidget>
#include <QTextDocument>
#include <QCloseEvent>
#include <QFile>
#include <QStandardPaths>
#include <QFileDialog>
#include <QTextStream>
#include <QShortcut>
#include <QFileInfo>
#include <QPlainTextEdit>

#include "coreedit.h"
#include "../corebox/corebox.h"
#include "../corebox/globalfunctions.h"
#include "../bookmarks/bookmarkdialog.h"
#include "../bookmarks/bookmarks.h"


namespace Ui {
class corepad;
}

class corepad : public QWidget
{
    Q_OBJECT

public:
    explicit corepad(QWidget *parent = 0);
    ~corepad();

    coreedit *text;
    bool initializeNewTab(QString filePath);

    void openText(QString filePath);
    void findS(QString searchS, bool reverse, QTextDocument::FindFlags flag = 0);
    QString fulldoc;
    QTextDocument *doc;
    QString workFilePath;

private slots:
    void on_text_copyAvailable(bool b);
    void on_text_undoAvailable(bool b);
    void on_text_redoAvailable(bool b);
    void on_text_textChanged();
    void on_cUndo_clicked();
    void on_cRedo_clicked();
    void on_cCut_clicked();
    void on_cCopy_clicked();
    void on_cPaste_clicked();
    void on_cSave_clicked();
    void on_cSaveAs_clicked();
    void on_cOpen_clicked();
    void on_cNew_clicked();
    void on_cQuit_clicked();
    void on_bookMarkIt_clicked();
    void on_fontSize_currentIndexChanged(const QString &arg1);
    void on_addDate_clicked();
    void on_search_clicked(bool checked);
    void on_searchHere_textChanged(const QString &arg1);
    void on_nextW_clicked();
    void on_previousW_clicked();
    void on_notes_currentChanged(int index);
    void on_notes_tabCloseRequested(int index);

public slots:
    void quiting();

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::corepad *ui;

    bool closeAllTab();
    bool isSaved;
    bool isUpdated;
    QFile *workingFile;
    QString documentPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    void shotcuts();

};

#endif // COREPAD_H
