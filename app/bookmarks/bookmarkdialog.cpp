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

#include "bookmarkdialog.h"
#include "ui_bookmarkdialog.h"

bookmarkDialog::bookmarkDialog(QWidget *parent) :QWidget(parent),
    ui(new Ui::bookmarkDialog)
{
    ui->setupUi(this);
}

bookmarkDialog::~bookmarkDialog()
{
    delete ui;
}

void bookmarkDialog::on_done_clicked()
{
    if (ui->bkName->text().count() == 0) {
        ui->done->setEnabled(false);
    }
    if (ui->bkName->text().count() != 0 && ui->bkSection->currentText().count() != 0) {
        accepted = true;
        QTimer::singleShot(100, this, SLOT(close()));
        messageEngine("Bookmark Added at '" + ui->bkSection->currentText() + "'", "Info");
    }
}

void bookmarkDialog::bookMarkName_Changed()
{
    if (ui->bkName->text().count() > 0) {
        QString str = bk.checkingBookName(ui->bkSection->currentText(), ui->bkName->text());
        if (str.count() > 0) {
            ui->bkStatus->setText(str);
            ui->done->setEnabled(false);
        } else {
            ui->bkStatus->setText(str);
            ui->done->setEnabled(true);
        }
    }
    else {
        ui->done->setEnabled(false);
    }
}

void bookmarkDialog::checkPath()
{
    QString str = bk.checkingBookPath(ui->bkSection->currentText(), ui->bkName->text());
    if (str.count() > 0) {
        ui->bkStatus->setText(str);
        ui->bkName->setEnabled(false);
        ui->done->setEnabled(false);
        ui->cancel->setText("OK");
    } else {
        ui->bkStatus->setText(str);
        ui->bkName->setEnabled(true);
        ui->done->setEnabled(true);
        ui->cancel->setText("Cancel");
    }
}

void bookmarkDialog::item_Changed()
{
    checkPath();
    bookMarkName_Changed();
}


void bookmarkDialog::on_bkSection_currentIndexChanged(const QString &arg1)
{
    checkPath();
    bookMarkName_Changed();
}

void bookmarkDialog::on_bkName_textChanged(const QString &arg1)
{
    if (ui->bkName->text().count() > 0) {
        QString str = bk.checkingBookName(ui->bkSection->currentText(), ui->bkName->text());
        if (str.count() > 0) {
            ui->bkStatus->setText(str);
            ui->done->setEnabled(false);
        } else {
            ui->bkStatus->setText(str);
            ui->done->setEnabled(true);
        }
    }
    else {
        ui->done->setEnabled(false);
    }
}
