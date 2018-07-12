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

#include "renamedialog.h"
#include "ui_renamedialog.h"

renameDialog::renameDialog(QFileInfo iFile,QWidget *parent):QDialog(parent),ui(new Ui::renameDialog)
{
    ui->setupUi(this);

    m_iFile = iFile;
    setWindowFlags(Qt::Dialog | Qt::Popup);
    setAttribute(Qt::WA_NativeWindow);
    setWindowIcon(geticon(m_iFile.filePath()));
    setWindowTitle("Rename \"" + m_iFile.fileName() + "\"");
    ui->newName->setText(m_iFile.fileName());
    ui->pic->setPixmap(geticon(iFile.filePath()).pixmap(120, 120));
    connect(ui->cancel, &QPushButton::clicked, this, &renameDialog::close);

    shotcuts();
}

renameDialog::~renameDialog()
{
    delete ui;
}

void renameDialog::shotcuts()
{
    QShortcut* shortcut;

    shortcut = new QShortcut(QKeySequence(Qt::Key_Enter), this);
    connect(shortcut, &QShortcut::activated, this, &renameDialog::on_done_clicked);
}

void renameDialog::on_done_clicked()
{
    if (ui->newName->text().count()) {
        if (QFile::rename(m_iFile.filePath(), m_iFile.path() + "/" + ui->newName->text())) {
            messageEngine("File Renamed Successfully.", MessageType::Info);
            close();
        } else {
            messageEngine("File not Renamed.", MessageType::Warning);
            close();
        }
    }
}
