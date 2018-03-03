#ifndef INFO_MANAGER_H
#define INFO_MANAGER_H

#include "cpu_info.h"
#include "memory_info.h"
#include "network_info.h"
#include "format_util.h"

#include "../info/system_info.h"

#include <QObject>


class InfoManager
{
public:
    static InfoManager *ins();

    quint8 getCpuCoreCount() const;
    QList<int> getCpuPercents() const;

    quint64 getSwapUsed() const;
    quint64 getSwapTotal() const;
    quint64 getMemUsed() const;
    quint64 getMemTotal() const;
    void updateMemoryInfo();

    quint64 getRXbytes() const;
    quint64 getTXbytes() const;
    void updateDiskInfo();

    QFileInfoList getCrashReports() const;
    QFileInfoList getAppLogs() const;
    QFileInfoList getAppCaches() const;

    void updateProcesses();
    QString getUserName() const;

private:
    InfoManager();
    static InfoManager *_instance;

    CpuInfo ci;
    MemoryInfo mi;
    NetworkInfo ni;
    SystemInfo si;

};

#endif // INFO_MANAGER_H
