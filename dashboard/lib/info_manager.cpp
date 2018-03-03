#include "info_manager.h"


InfoManager::InfoManager()
{}

InfoManager *InfoManager::_instance = NULL;

InfoManager *InfoManager::ins()
{
    if(_instance == NULL)
        _instance = new InfoManager;

    return _instance;
}

QString InfoManager::getUserName() const
{
    return si.getUsername();
}

/********************
 * CPU INFORMATION
 *******************/
quint8 InfoManager::getCpuCoreCount() const
{
    return ci.getCpuCoreCount();
}

QList<int> InfoManager::getCpuPercents() const
{
    return ci.getCpuPercents();
}

/********************
 * MEMORY INFORMATION
 *******************/
void InfoManager::updateMemoryInfo()
{
    mi.updateMemoryInfo();
}

quint64 InfoManager::getSwapUsed() const
{
    return mi.getSwapUsed();
}

quint64 InfoManager::getSwapTotal() const
{
    return mi.getSwapTotal();
}

quint64 InfoManager::getMemUsed() const
{
    return mi.getMemUsed();
}

quint64 InfoManager::getMemTotal() const
{
    return mi.getMemTotal();
}


/********************
 * NETWORK INFORMATION
 *******************/
quint64 InfoManager::getRXbytes() const
{
    return ni.getRXbytes();
}

quint64 InfoManager::getTXbytes() const
{
    return ni.getTXbytes();
}



