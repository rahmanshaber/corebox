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

#include "dashboard.h"
#include "ui_dashboard.h"


dashboard::dashboard(QWidget *parent) :QWidget(parent),ui(new Ui::dashboard)
{
    ui->setupUi(this);

    pgeneral *infoo = new pgeneral(this);
    QHBoxLayout *hl = new QHBoxLayout();
    hl->setContentsMargins(0, 0, 0, 0);
    hl->addWidget(infoo);
    ui->pages->widget(0)->setLayout(hl);

    pDrives *drives = new pDrives(this);
    ui->pDriveLayout->addWidget(drives);

    pBattery *battery = new pBattery(this);
    ui->pBatteryLayout->addWidget(battery);

    pDisplay *display = new pDisplay(this);
    ui->pDisplayLayout->addWidget(display);

    presources *resourcePage = new presources(this);
    QHBoxLayout *hl2 = new QHBoxLayout();
    hl2->setContentsMargins(0, 0, 0, 0);
    hl2->addWidget(resourcePage);
    ui->pages->widget(3)->setLayout(hl2);

    ui->pages->setCurrentIndex(0);
}

dashboard::~dashboard()
{
    delete ui;
}

void dashboard::pageClick(QPushButton *btn, int i, QString title)
{
    // all button checked false
    for (QPushButton *b : ui->shortcut->findChildren<QPushButton*>())
        b->setChecked(false);
    btn->setChecked(true);
    ui->selectedsection->setText(title);
    ui->pages->setCurrentIndex(i);
}

void dashboard::on_Bresource_clicked()
{
    pageClick(ui->Bresource, 3, tr("Resource"));
}

void dashboard::on_Bdrives_clicked()
{
    pageClick(ui->Bdrives, 1, tr("Drives"));
}

void dashboard::on_Bbattery_clicked()
{
    pageClick(ui->Bbattery, 2, tr("Battery"));
}

void dashboard::on_Bgeneral_clicked()
{
    pageClick(ui->Bgeneral, 0, tr("General"));
}

void dashboard::on_Bdisplay_clicked()
{
    pageClick(ui->Bdisplay, 4, tr("Display"));
}

void dashboard::reload()
{
//    battery->reload();
}
