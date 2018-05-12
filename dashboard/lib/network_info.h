#ifndef NETWORK_INFO_H
#define NETWORK_INFO_H

#include "command_util.h"
#include "stacer-core_global.h"

#define PROC_NET_ROUTE "/proc/net/route"


class STACERCORESHARED_EXPORT NetworkInfo
{
public:
    NetworkInfo();

    QString getDefaultNetworkInterface() const;

    quint64 getRXbytes() const;
    quint64 getTXbytes() const;

private:
    QString defaultNetworkInterface;

    QString rxPath;
    QString txPath;
};

#endif // NETWORK_INFO_H
