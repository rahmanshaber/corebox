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

#include "wnotes.h"
#include "ui_wnotes.h"


wNotes::wNotes(QWidget *parent) :QWidget(parent),ui(new Ui::wNotes)
{
    ui->setupUi(this);
    collectNotes();
}

wNotes::~wNotes()
{
    delete ui;
}

void wNotes::collectNotes()
{
    QSettings notes(QDir::homePath() + "/.config/coreBox/Notes", QSettings::IniFormat);
    notes.beginGroup("Notes");
    ui->plainTextEdit->setPlainText(notes.value("1").toString());
//    ui->plainTextEdit_2->setPlainText(notes.value("2").toString());
    notes.endGroup();
    notes.sync();
}

bool wNotes::saveNotes()
{
    QSettings notes(QDir::homePath() + "/.config/coreBox/Notes", QSettings::IniFormat);
    if (notes.status() == QSettings::NoError) {
        notes.beginGroup("Notes");
        notes.setValue("1", ui->plainTextEdit->toPlainText());
//        notes.setValue("2", ui->plainTextEdit_2->toPlainText());
        notes.endGroup();
        notes.sync();
        return true;
    } else {
        return false;
    }
}

void wNotes::closeEvent(QCloseEvent *event)
{
    event->ignore();
    if (saveNotes()) event->accept();
    else event->ignore();
}
