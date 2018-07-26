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

#include "about.h"
#include "ui_about.h"


about::about(QWidget *parent) : QWidget(parent), ui(new Ui::about)
{
    ui->setupUi(this);

    // set stylesheet from style.qrc
    setStyleSheet(getStylesheetFileContent(":/appStyle/style/About.qss"));

    on_corebox_clicked();
}

about::~about()
{
    delete ui;
}

void about::pageClick(QPushButton *btn, int i, QString title)
{
    // all button checked false
    for (QPushButton *b : ui->shortcut->findChildren<QPushButton*>())
        b->setChecked(false);
    btn->setChecked(true);
    ui->selectedsection->setText(title);
    ui->pages->setCurrentIndex(i);
}

void about::on_corebox_clicked()
{
    pageClick(ui->corebox,0, tr("CoreBox"));
}

void about::on_releaseNotes_clicked()
{
    pageClick(ui->releaseNotes,1, tr("Relese Notes"));
    QFile p(":/docs/ReleaseNotes");
    p.open(QFile::ReadOnly | QFile::Text );
    QTextStream o(&p);
    ui->pgrelesenotes->setText(o.readAll());
}

void about::on_helpUs_clicked()
{
    pageClick(ui->helpUs,2, tr("Help Us"));
}
