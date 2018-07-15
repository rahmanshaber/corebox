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

#ifndef TEXTDIALOG_H
#define TEXTDIALOG_H

#include "../imagearea.h"
#include "../datasingleton.h"

#include <QDialog>
#include <QTextEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFont>
#include <QFontDialog>
#include <QDialogButtonBox>
#include <QMessageBox>


class TextDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TextDialog(QString text, ImageArea *parent);
   
private:
    void initializeGui();
    QTextEdit *mTextEdit;
  
signals:
    void textChanged(ImageArea *, const QString);
    void canceled(ImageArea *);

private slots:
    void textChanged();
    void selectFont();    
    void cancel();
    void reject();

};

#endif // TEXTDIALOG_H
