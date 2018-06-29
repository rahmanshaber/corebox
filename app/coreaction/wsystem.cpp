#include "wsystem.h"
#include "ui_wsystem.h"
#include <QTimer>

wSystem::wSystem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::wSystem),
    info(InfoManager::ins())
{
    ui->setupUi(this);
    checkSystem();
    QTimer *timer = new QTimer();
    timer->start(1000);

    connect(timer, &QTimer::timeout, this, &wSystem::checkSystem);
}

wSystem::~wSystem()
{
    delete ui;
}

void wSystem::checkSystem() {
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
