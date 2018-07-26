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

#ifndef PRESOURCES_H
#define PRESOURCES_H

#include <QWidget>
#include <QTimer>

#include "dashboard/utilities.h"
#include "dashboard/history_chart.h"
#include "dashboard/info_manager.h"


namespace Ui {
class presources;
}

class presources : public QWidget
{
    Q_OBJECT

public:
    explicit presources(QWidget *parent = 0);
    ~presources();

private slots:
    void updateCpuChart();
    void updateCpuLoadAvg();
    void updateDiskReadWrite();
    void updateMemoryChart();
    void updateNetworkChart();

private:
    Ui::presources *ui;

    InfoManager *im;
    QTimer *mTimer;
    void init();

    HistoryChart *mChartCpu;
    HistoryChart *mChartCpuLoadAvg;
    HistoryChart *mChartDiskReadWrite;
    HistoryChart *mChartMemory;
    HistoryChart *mChartNetwork;

};

#endif // PRESOURCES_H
