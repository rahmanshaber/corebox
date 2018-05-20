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

#include "sysinfo.h"
#include "ui_sysinfo.h"

#include "system_info.h"
#include "corebox/globalfunctions.h"

#include <QStringListModel>
#include <QSysInfo>
#include <QtNetwork>
#include <QFontMetrics>
#include <QFont>
#include <QObject>

sysinfo::sysinfo(QWidget *parent) :QWidget(parent),ui(new Ui::sysinfo),
  cpuBar(new CircleBar(tr("CPU"), {"#A8E063", "#56AB2F"}, this)),
  memBar(new CircleBar(tr("MEMORY"), {"#FFB75E", "#ED8F03"}, this)),
  timer(new QTimer(this)),
  im(InfoManager::ins())
{
    ui->setupUi(this);

    init();
    systemInformationInit();
}

sysinfo::~sysinfo()
{
    delete ui;
}

void sysinfo::init()
{
    // Circle bars
    ui->circleBarsLayout->addWidget(cpuBar);
    ui->circleBarsLayout->addWidget(memBar);
    ui->circleBarsLayout->addWidget(ui->network);

    // connections
    connect(timer, &QTimer::timeout, this, &sysinfo::updateCpuBar);
    connect(timer, &QTimer::timeout, this, &sysinfo::updateMemoryBar);
    connect(timer, &QTimer::timeout, this, &sysinfo::updateNetworkBar);

    timer->start(1 * 1000);

    // initialization
    updateCpuBar();
    updateMemoryBar();
    updateNetworkBar();
}

void sysinfo::systemInformationInit()
{
    QDesktopWidget dw;

    QString scr = QString::number(dw.width()) + " x " + QString::number(dw.height()) + " px";

    // get system information
    SystemInfo sysInfo;

//    QStringList infos;
//    infos
//        << tr("UserName : %1")            .arg(sysInfo.getHostname())
//        << tr("Platform : %1")            .arg(sysInfo.getPlatform())
//        << tr("Distribution : %1")        .arg(sysInfo.getDistribution())
//        << tr("Kernel Release : %1")      .arg(sysInfo.getKernel())
//        << tr("CPU Model : %1")           .arg(sysInfo.getCpuModel())
//        << tr("CPU Speed : %1 (Mhz)")     .arg(sysInfo.getCpuSpeed())
//        << tr("CPU Core  : %1")           .arg(sysInfo.getCpuCore())
//        << tr("Display Size : %1")        .arg(scr)
//        << tr("Qt Toolkit Version : %1")  .arg(QT_VERSION_STR);

    QStringList left;
    left << "User Name" << "Platform" << "Distribution" << "Kernel Release"
         << "CPU Model" << "CPU Speed" << "CPU Core" << "Display Size"
         << "Qt Toolkit Version";

    QStringList right;
    right << sysInfo.getHostname() << sysInfo.getPlatform() << sysInfo.getDistribution()
          << sysInfo.getKernel() << sysInfo.getCpuModel() << sysInfo.getCpuSpeed()
          << sysInfo.getCpuCore() << scr << QT_VERSION_STR;

    QStringListModel *systemInfoModel = new QStringListModel(fStringList(left, right, ui->systemInfoList->font()));

    ui->systemInfoList->setModel(systemInfoModel);
}

void sysinfo::updateCpuBar()
{
    int cpuUsedPercent = im->getCpuPercents().at(0);

    cpuBar->setValue(cpuUsedPercent, QString("%1%").arg(cpuUsedPercent));
}

void sysinfo::updateMemoryBar()
{
    im->updateMemoryInfo();

    int memUsedPercent = 0;
    if (im->getMemTotal())
        memUsedPercent = ((double)im->getMemUsed() / (double)im->getMemTotal()) * 100.0;

    QString f_memUsed  = formatSize(im->getMemUsed());
    QString f_memTotal = formatSize(im->getMemTotal());

    memBar->setValue(memUsedPercent, QString("%1 / %2").arg(f_memUsed).arg(f_memTotal));
}

void sysinfo::updateNetworkBar()
{
    QNetworkAccessManager manager;
    const auto config = manager.activeConfiguration();
//    qDebug() << config.bearerType() << config.bearerTypeName();
    ui->usedN->setText(config.bearerTypeName());


    static quint64 l_RXbytes = im->getRXbytes();
    static quint64 l_TXbytes = im->getTXbytes();
    static quint64 max_RXbytes = 1L << 20; // 1 MEBI
    static quint64 max_TXbytes = 1L << 20; // 1 MEBI

    quint64 RXbytes = im->getRXbytes();
    quint64 TXbytes = im->getTXbytes();

    quint64 d_RXbytes = (RXbytes - l_RXbytes);
    quint64 d_TXbytes = (TXbytes - l_TXbytes);

    QString downText = formatSize(d_RXbytes);
    QString upText = formatSize(d_TXbytes);

    int downPercent = ((double) d_RXbytes / (double) max_RXbytes) * 100.0;
    int upPercent = ((double) d_TXbytes / (double) max_TXbytes) * 100.0;

    ui->downloadBar->setValue(downPercent);

    ui->uploadBar->setValue(upPercent);

    ui->dspeed->setText(downText);
    ui->uspeed->setText(upText);
    ui->dtotal->setText(formatSize(RXbytes));
    ui->utotal->setText(formatSize(TXbytes));

    max_RXbytes = qMax(max_RXbytes, d_RXbytes);
    max_TXbytes = qMax(max_TXbytes, d_TXbytes);

    l_RXbytes = RXbytes;
    l_TXbytes = TXbytes;
}

