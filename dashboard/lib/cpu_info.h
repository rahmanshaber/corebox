#ifndef CPUINFO_H
#define CPUINFO_H

#include "stacer-core_global.h"
#include "command_util.h"

#define PROC_CPUINFO "/proc/cpuinfo"
#define PROC_STAT    "/proc/stat"

#include <QDebug>
#include <QVector>


class STACERCORESHARED_EXPORT CpuInfo
{

public:
    CpuInfo();

    quint8 getCpuCoreCount() const;
    QList<int> getCpuPercents() const;

private:
    int getCpuPercent(const QList<double> &cpuTimes, const int &processor = 0) const;

};

#endif // CPUINFO_H
