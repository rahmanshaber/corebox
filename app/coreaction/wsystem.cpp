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

#include "wsystem.h"
#include "ui_wsystem.h"
#include "corebox/globalfunctions.h"


wSystem::wSystem(QWidget *parent) :QWidget(parent),ui(new Ui::wSystem),
    info(InfoManager::ins())
{
    ui->setupUi(this);
    checkSystem();
    QTimer *timer = new QTimer();
    timer->start(1000);

    addDropShadow(this, 60, 50);

    connect(timer, &QTimer::timeout, this, &wSystem::checkSystem);
}

wSystem::~wSystem()
{
    delete ui;
}

void wSystem::checkSystem()
{
    //set cpu value
    int cpuPercent = 0;
    QList<int> percentL = info->getCpuPercents();
    for (int i = 0; i < percentL.count(); i++) {
        cpuPercent += percentL.at(i);
    }
    cpuPercent = cpuPercent / percentL.count();

    ui->cpubar->setValue(cpuPercent);

    //set ram bar value
    info->updateMemoryInfo();
    int memPercent = 0;

    if (info->getMemTotal())
        memPercent = ((double)info->getMemUsed() / (double)info->getMemTotal()) * 100.0;

    ui->rambar->setValue(memPercent);
}
