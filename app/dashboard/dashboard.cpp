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
    on_Bgeneral_clicked();
}

dashboard::~dashboard()
{
    delete ui->pages;
    delete ui;

}

void dashboard::closeEvent(QCloseEvent *event)
{
    event->ignore();
    for (int i = 0; i < ui->pages->count(); i++) {
        if (ui->pages->widget(i)->children().at(1))
            ui->pages->widget(i)->children().at(1)->deleteLater();
    }
    event->accept();
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

void dashboard::on_Bgeneral_clicked()
{
    if (ui->pages->widget(0)->children().count() < 2) {
        pgeneral *infoo = new pgeneral(this);
        ui->pGeneralLayout->addWidget(infoo);
    }

    pageClick(ui->Bgeneral, 0, tr("General"));
}

void dashboard::on_Bdrives_clicked()
{
    if (ui->pages->widget(1)->children().count() < 2) {
        pDrives *drives = new pDrives(this);
        ui->pDriveLayout->addWidget(drives);
    }

    pageClick(ui->Bdrives, 1, tr("Drives"));
}

void dashboard::on_Bbattery_clicked()
{
    if (ui->pages->widget(2)->children().count() < 2) {
        pBattery *battery = new pBattery(this);
        ui->pBatteryLayout->addWidget(battery);
    }

    pageClick(ui->Bbattery, 2, tr("Battery"));
}

void dashboard::on_Bresource_clicked()
{
    if (ui->pages->widget(3)->children().count() < 2) {
        presources *resourcePage = new presources(this);
        ui->pResourceLayout->addWidget(resourcePage);
    }

    pageClick(ui->Bresource, 3, tr("Resource"));
}

void dashboard::on_Bdisplay_clicked()
{
    if (ui->pages->widget(4)->children().count() < 2) {
        pDisplay *display = new pDisplay(this);
        ui->pDisplayLayout->addWidget(display);
    }

    pageClick(ui->Bdisplay, 4, tr("Display"));
}

