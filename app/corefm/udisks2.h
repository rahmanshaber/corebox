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

#ifndef UDISKS2_H
#define UDISKS2_H

#include <QObject>
#include <QtDBus/QDBusObjectPath>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusReply>
#include <QtDBus/QDBusConnection>
#include <QXmlStreamReader>

#include "corebox/globalfunctions.h"


class UDisks2Block;
class UDisks2Drive;
class UDisks2Filesystem;

class UDisks2 : public QObject {
    Q_OBJECT

public:
    explicit UDisks2(QObject *parent = NULL);
    ~UDisks2();

    QStringList blockDevices();
    UDisks2Block *blockDevice(const QString &node);

    QStringList drives();
    UDisks2Drive *drive(const QString &node);

signals:
    void deviceInformationChanged(QString node, QVariantMap info);
    void driveAdded(const QString& node);
    void driveRemoved(const QString& node);
    void driveChanged(const QString& node);
    void blockDeviceAdded(const QString& node);
    void blockDeviceRemoved(const QString &node);
    void blockDeviceChanged(const QString &node);
    void filesystemAdded(const QString& node);
    void filesystemRemoved(const QString &node);
    void filesystemChanged(const QString &node);

private:
    void addDrive(const QString &node);
    void addBlock(const QString &node);
    void removeDrive(const QString &node);
    void removeBlock(const QString &node);

    QMap<QString,UDisks2Drive*> drives_;
    QMap<QString,UDisks2Block*> blocks_;

private slots:
    void dbus_interfaceAdded(const QDBusObjectPath &path, const QMap<QString, QVariant> &interfaces);
    void dbus_interfaceRemoved(const QDBusObjectPath &path, const QStringList &interfaces);
};



class UDisks2Block : public QObject {
    Q_OBJECT
public:
    explicit UDisks2Block(const QString &node, QObject *parent = NULL);

public:
    QString name;
    QString dev;
    QString id;
    QString drive;
    qulonglong size;
    bool readonly;
    QString usage;
    QString type;
    QString toString();
    QString toStringToSeperate(int i);

    void update();
    void updateFilesystem();
    void addFilesystem();
    void removeFilesystem();
    UDisks2Filesystem *fileSystem();

signals:
    void filesystemAdded(const QString& node);
    void filesystemRemoved(const QString &node);
    void filesystemChanged(const QString &node);
    void changed(const QString &node);

private slots:
    void self_propertiesChanged(const QString &interface, const QVariantMap &changedProp, const QStringList &invalidatedProp);

private:
    QDBusInterface *dbus;
    UDisks2Filesystem* fs;
};

class UDisks2Filesystem : public QObject {
    Q_OBJECT
public:
    UDisks2Filesystem(const QString &node, QObject *parent = NULL);
    QStringList mountPoints() const;
    QString mount();
    void unmount();
    void update();
    bool isValid();
    QString name;

private:
    QDBusInterface *dbus;
    QDBusInterface *dbusProp;
    QStringList mountPoints_;
};

class UDisks2Drive : public QObject {
    Q_OBJECT
public:
    explicit UDisks2Drive(const QString &node, QObject *parent = NULL);

    QString name;
    qulonglong size;
    QString vendor;
    QString model;
    QString serial;
    QString id;
    QString media;
    bool optical;
    bool removable;
    bool available;
    QString toString();
    QString toStringToSeperate(int i);

    void update();

signals:
    void changed(const QString &node);

private slots:
    void self_propertiesChanged(const QString &interface, const QVariantMap &changed, const QStringList &invalidated);

private:
    QDBusInterface *dbus;
};


#endif // UDISKS2_H
