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


bookmarkDialog::bookmarkDialog(QWidget *parent) : QDialog(parent),ui(new Ui::bookmarkDialog)
{
    ui->setupUi(this);

    connect(ui->cancel, &QPushButton::clicked, this, &bookmarkDialog::close);

    ui->bkSection->clear();
    ui->bkSection->addItems(bk.getBookSections());
    ui->done->setEnabled(false);
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
        // Function from globalfunctions.cpp
        messageEngine("Bookmark Added at '" + ui->bkSection->currentText() + "'", MessageType::Info);
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

void bookmarkDialog::setBookIcon(QPixmap pix) {
    ui->bkIcon->setPixmap(pix);
}

void bookmarkDialog::setBookPath(const QString &path) {
    ui->path->setText(path);
}

void bookmarkDialog::setBookName(const QString &bName) {
    ui->bkName->setText(bName);
}

QString bookmarkDialog::getBookName() {
    return ui->bkName->text();
}

QString bookmarkDialog::getSectionName() {
    return ui->bkSection->currentText();
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
