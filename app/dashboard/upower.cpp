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

#include "upower.h"


const QString UPower::m_consolkit_interface = "org.freedesktop.ConsoleKit";
const QString UPower::m_consolkit_path      = "/org/freedesktop/ConsoleKit/Manager";

const QString UPower::m_upower_interface    = "org.freedesktop.UPower";
const QString UPower::m_upower_path         = "/org/freedesktop/UPower";

UPower::UPower(QObject *parent) : QObject(parent), m_available(false), m_interface(0) {
    connect(this, SIGNAL(available()), this, SLOT(refreshBatteries()));
    connect(this, SIGNAL(unavailable()), this, SLOT(clearBattery()));

    m_batteries = new QMap<QString, Battery*>;

    createDBusWatcher();
    createDBusInterface();
}

UPower::~UPower() {
    m_interface->deleteLater();
}

void UPower::createDBusWatcher() {
    m_watcher = new QDBusServiceWatcher;
    m_watcher->addWatchedService("org.freedesktop.UPower");
    m_watcher->setConnection(QDBusConnection::systemBus());
    m_watcher->setWatchMode(QDBusServiceWatcher::WatchForRegistration | QDBusServiceWatcher::WatchForUnregistration);

    connect(m_watcher, SIGNAL(serviceUnregistered(QString)), this, SIGNAL(unavailable()));
    connect(m_watcher, SIGNAL(serviceRegistered(QString)), this, SLOT(createDBusInterface()));
}

void UPower::createDBusInterface() {
    if(m_interface==0 || !m_interface->isValid()) {
        m_interface = new QDBusInterface(m_upower_interface, m_upower_path, m_upower_interface,
                                         QDBusConnection::systemBus());
        if(!m_interface->isValid()) {
            m_interface->deleteLater();
            m_interface = 0;
            m_available = false;
            return;
        }

        connect(m_interface, SIGNAL(DeviceAdded(QDBusObjectPath)), this, SLOT(addBattery(QDBusObjectPath)));
        connect(m_interface, SIGNAL(DeviceRemoved(QDBusObjectPath)), this, SLOT(removeBattery(QDBusObjectPath)));
    }

    m_available = true;
    emit available();
}

void UPower::refreshBatteries() {
    QDBusArgument argument = m_interface->call( "EnumerateDevices" ).arguments().at(0).value<QDBusArgument>();

    if( m_interface->lastError().type() == QDBusError::NoError ) {
        argument.beginArray();
        while( !argument.atEnd() ) {
            QDBusObjectPath path;
            argument >> path;
            addBattery(path);
        }
    }
}

void UPower::clearBattery() {
    m_batteries->clear();
}

bool UPower::addBattery(const QDBusObjectPath &path) {
    Battery *battery = new Battery(path.path());

    if(addBattery(battery)) {
        return true;
    }

    battery->deleteLater();
    return false;
}

bool UPower::addBattery(Battery *battery) {
    if(battery->isValid() && !m_batteries->contains(battery->path())) {
        m_batteries->insert(battery->path(), battery);
        emit batteryConnected(battery);
        emit batteryConnected(battery->path());
        return true;
    }

    return false;
}

void UPower::removeBattery(Battery *battery) {
    m_batteries->remove(m_batteries->key(battery));
    emit batteryDisconnected(battery);
    emit batteryDisconnected(battery->path());
    battery->deleteLater();
}

void UPower::removeBattery(const QDBusObjectPath &path) {
    removeBattery(m_batteries->value(path.path(), 0));
}

Battery *UPower::battery(const QString &path) const {
    return m_batteries->value(path, 0);
}

int UPower::numberOfBattery() const {
    return m_batteries->size();
}

bool UPower::isAvailable() const {
    return m_available;
}

const QMap<QString, Battery*> *UPower::batteries() const {
    return m_batteries;
}

const QString UPower::version() const {
    return m_interface->property("DaemonVersion").toString();
}

void UPower::suspendSystem() {
    if(systemCanSuspend()) {
        m_interface->callWithCallback("Suspend", QList<QVariant>(), this, SIGNAL(systemSuspend()));
        emit systemSuspend(true);
    } else {
        emit systemSuspend(false);
    }
}

void UPower::hibernateSystem() {
    if(systemCanHibernate()) {
        m_interface->callWithCallback("Hibernate", QList<QVariant>(), this, SIGNAL(systemHibernate()));
        emit systemHibernate(true);
    } else {
        emit systemHibernate(false);
    }
}

void UPower::rebootSystem() const {
    QDBusInterface interface(m_consolkit_interface, m_consolkit_path,
                             m_consolkit_interface,QDBusConnection::systemBus());
    interface.call("Restart");
}

void UPower::haltSystem() const {
    QDBusInterface interface(m_consolkit_interface, m_consolkit_path,
                             m_consolkit_interface, QDBusConnection::systemBus());
    interface.call("Stop");
}

bool UPower::systemCanHibernate() const {
    return m_interface->property("CanHibernate").toBool();
}

bool UPower::systemCanSuspend() const {
    return m_interface->property("CanSuspend").toBool();
}

bool UPower::isOnBattery() const {
    return m_interface->property("OnBattery").toBool();
}
bool UPower::isLowBattery() const {
    return m_interface->property("OnLowBattery").toBool();
}

bool UPower::isLidClosed() const {
    return m_interface->property("LidIsClosed").toBool();
}

bool UPower::isLidPresent() const {
    return m_interface->property("LidIsPresent").toBool();
}
