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

#include "help.h"
#include "ui_help.h"

#include <QTextStream>
#include <QFile>

help::help(QWidget *parent) :QWidget(parent),ui(new Ui::help)
{
    ui->setupUi(this);
    on_helpus_clicked();
}

help::~help()
{
    delete ui;
}

void help::pageClick(QPushButton *btn, int i, QString title)
{
    // all button checked false
    for (QPushButton *b : ui->shortcut->findChildren<QPushButton*>())
        b->setChecked(false);
    btn->setChecked(true);
    ui->selectedsection->setText(title);
    ui->pages->setCurrentIndex(i);
}

void help::on_helpus_clicked()
{
    pageClick(ui->helpus,12, tr("HelpUs"));
}

void help::on_search_clicked()
{
    pageClick(ui->search,11, tr("Search"));
}

void help::on_start_clicked()
{
    pageClick(ui->start,10, tr("Start"));
}

void help::on_dashboard_clicked()
{
    pageClick(ui->dashboard,9, tr("DashBoard"));
}

void help::on_coreplayer_clicked()
{
    pageClick(ui->coreplayer,8, tr("CorePlayer"));
}

void help::on_corerenamer_clicked()
{
    pageClick(ui->corerenamer,7, tr("CoreRenamer"));
}

void help::on_coretime_clicked()
{
    pageClick(ui->coretime,6, tr("CoreTime"));
}

void help::on_corescrshot_clicked()
{
    pageClick(ui->corescrshot,5, tr("CoreScreenShot"));
}

void help::on_corepaint_clicked()
{
    pageClick(ui->corepaint,4, tr("CorePaint"));
}

void help::on_coreimage_clicked()
{
    pageClick(ui->coreimage,3, tr("CoreImage"));
}

void help::on_corepad_clicked()
{
    pageClick(ui->corepad,2, tr("CorePad"));
}

void help::on_corefm_clicked()
{
    pageClick(ui->corefm,1, tr("CoreFM"));
}

void help::on_corebox_clicked()
{
    pageClick(ui->corebox,0, tr("CoreBox"));
}

void help::on_relesenotes_clicked()
{
    pageClick(ui->relesenotes,13, tr("Relese Notes"));
    QFile p(":/docs/ReleaseNotes");
    p.open(QFile::ReadOnly | QFile::Text );
    QTextStream o(&p);
    ui->pgreleasenotes->setText(o.readAll());
}
