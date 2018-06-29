#ifndef DISKINFO_H
#define DISKINFO_H

#include "Utils/command_util.h"
#include "Utils/file_util.h"
#include <QStorageInfo>
#include "libcsys_global.h"

#define PROC_MOUNTS "/proc/mounts"

class Disk;

class LIBCSYSSHARED_EXPORT DiskInfo
{
public:
    QList<Disk *> getDisks() const;
    void updateDiskInfo();
    QList<quint64> getDiskIO() const;
    QString getDiskName() const;

private:

};

class Disk {
public:
    QString name;
    QString device;
    quint64 size;
    quint64 free;
    quint64 used;
};


#endif // DISKINFO_H
