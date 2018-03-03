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

#include "battery.h"

#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusInterface>
#include <QSettings>


Battery::Battery(const QString &path, QObject *parent)
    : QObject(parent), m_interface(0),
      m_path(path), m_hasAlreadyBeenLow(false),
      m_hasAlreadyBeenFull(false), m_valid(false)  {

    createInterface();
}

Battery::~Battery() {

}

void Battery::createInterface() {
    if(m_interface==0 || !m_interface->isValid()) {
        m_interface = new QDBusInterface("org.freedesktop.UPower", m_path,
                                         "org.freedesktop.UPower.Device", QDBusConnection::systemBus(), this);
        connect(m_interface, SIGNAL(Changed()), this, SIGNAL(changed()));
        if(!m_interface->isValid()) {
            delete m_interface;
            m_interface = 0;
            m_valid = false;
            return;
        }
    }

    m_valid = true;
}

void Battery::refresh() {
    m_interface->call("Refresh");
}

void Battery::update() {
    int level = percentage();

    if(level<lowLevel()) {
        if(!m_hasAlreadyBeenLow) {
            m_hasAlreadyBeenLow = true;
            emit low();
        }
    } else {
        m_hasAlreadyBeenLow = false;
    }

    if(state() == FullyCharged) {
        if(!m_hasAlreadyBeenFull) {
            m_hasAlreadyBeenFull = true;
            emit full();
        }
    } else {
        m_hasAlreadyBeenFull = false;
    }
}

bool Battery::isValid() const {
    return m_interface->property("Type").toInt() == 2 && m_valid;
}

int Battery::lowLevel() const {
    QSettings settings;
    settings.beginGroup("Batteries_Low_Level");
        int lvl = settings.value(m_path, 20).toInt();
    settings.endGroup();
    return lvl;
}

void Battery::setLowLevel(int value) {
    QSettings settings;
    settings.beginGroup("Batteries_Low_Level");
        settings.setValue(m_path, value);
    settings.endGroup();
}

QString Battery::sysfsPath() const {
    return m_interface->property("NativePath").toString();
}

QString Battery::model() const {
    return m_interface->property("Model").toString();
}

QString Battery::vendor() const {
    return m_interface->property("Vendor").toString();
}

QString Battery::technology() const {
    return m_interface->property("Technology").toString();
}

const QString & Battery::path() const {
    return m_path;
}

bool Battery::powerSupply() const {
    return m_interface->property("PowerSupply").toBool();
}

bool Battery::hasHistory() const {
    return m_interface->property("HasHistory").toBool();
}

bool Battery::hasStatistics() const {
    return m_interface->property("HasStatistics").toBool();
}

bool Battery::isPresent() const {
    return m_interface->property("IsPresent").toBool();
}

bool Battery::isRechargeable() const {
    return m_interface->property("IsRechargeable").toBool();
}

double Battery::energy() const {
    return m_interface->property("Energy").toDouble();
}

double Battery::energyEmpty() const {
    return m_interface->property("EnergyEmpty").toDouble();
}

double Battery::energyFull() const {
    return m_interface->property("EnergyFull").toDouble();
}

double Battery::energyFullDesign() const {
    return m_interface->property("EnergyFullDesign").toDouble();
}

double Battery::energyRate() const {
    return m_interface->property("EnergyRate").toDouble();
}

double Battery::voltage() const {
    return m_interface->property("Voltage").toDouble();
}

double Battery::percentage() const {
    return m_interface->property("Percentage").toDouble();
}

double Battery::capacity() const {
    return m_interface->property("Capacity").toDouble();
}

double Battery::toEmpty() const {
    return m_interface->property("TimeToEmpty").toDouble();
}

double Battery::toFull() const {
    return m_interface->property("TimeToFull").toDouble();
}

Battery::State Battery::state() const {
    uint state = m_interface->property("State").toUInt();
    switch( state ) {
        case (uint) 0:
            return FullyCharged;
            break;

        case (uint) 1:
            return Charging;
            break;

        case (uint) 2:
            return Discharging;
            break;

        default:
            return FullyCharged;
            break;
    }
}
