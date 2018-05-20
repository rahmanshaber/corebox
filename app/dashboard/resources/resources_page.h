#ifndef RESOURCESPAGE_H
#define RESOURCESPAGE_H

#include "history_chart.h"

#include "../lib/info_manager.h"

#include <QWidget>
#include <QTimer>


namespace Ui {
    class ResourcesPage;
}

class ResourcesPage : public QWidget
{
    Q_OBJECT

public:
    explicit ResourcesPage(QWidget *parent = 0);
    ~ResourcesPage();

private slots:
    void updateCpuChart();
    void updateMemoryChart();
    void updateNetworkChart();

private:
    Ui::ResourcesPage *ui;
    void init();

    HistoryChart *cpuChart;
    HistoryChart *memoryChart;
    HistoryChart *networkChart;

    QTimer *timer;
    InfoManager *im;
};

#endif // RESOURCESPAGE_H
