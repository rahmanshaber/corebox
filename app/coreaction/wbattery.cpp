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

#include "wbattery.h"
#include "ui_wbattery.h"


wBattery::wBattery(QWidget *parent) :QWidget(parent),ui(new Ui::wBattery)
{
    ui->setupUi(this);
    addDropShadow(this, 40);

    ui->batteryProg->setAlignment(Qt::AlignCenter);
    u = new UPower(this);

    batteryCheck();

    QTimer *timer = new QTimer();
    timer->start(1000);

    connect(timer, &QTimer::timeout, this, &wBattery::batteryCheck);
}

wBattery::~wBattery()
{
    delete ui;
}

void wBattery::batteryCheck() {
    Battery *b = nullptr;
    foreach (Battery *bat, *u->batteries()) {
        b = u->batteries()->value(bat->path());
    }
    if (!b)
        return;

    QTime time(0, 0, 0, 0);

    double energy = b->energy();
    double energyEmpty = b->energyEmpty();
    double energyFull = b->energyFull();
    double rate = b->energyRate();
    int addSeconds = 0;

    ui->batteryProg->setValue(b->percentage());

    switch( b->state() ) {
        case Battery::FullyCharged:
            ui->batteryStatus->setText( tr( "Full" ));
            break;
        case Battery::Discharging:
            addSeconds = b->toEmpty();
            addSeconds = rate != 0 && addSeconds == 0 ? ( energy - energyEmpty ) * 4 / rate: addSeconds;
            time = time.addSecs(addSeconds);
            ui->batteryStatus->setText("Discharging (" + time.toString("hh %1 mm %2").arg("hours", "minutes)"));
            break;
        case Battery::Charging:
            addSeconds = b->toFull();
            addSeconds = rate != 0 && addSeconds == 0 ? ( energyFull - energy ) * 4 / rate : addSeconds;
            time = time.addSecs(addSeconds);
            ui->batteryStatus->setText("Charging(" + time.toString("hh %1 mm %2").arg("hours", "minutes)"));
            break;
        default:
            ui->batteryStatus->setText( tr( "No Battery" ) );
            break;
    }
}
