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

#include "dashboard.h"
#include "battery.h"
#include "upower.h"
#include "ui_dashboard.h"

#include <QMetaObject>
#include <QMetaProperty>
#include <QTreeWidgetItem>
#include <QDir>


dashboard::dashboard(QWidget *parent) :QWidget(parent),ui(new Ui::dashboard)
{
    qDebug() << "dashboard opening";
    ui->setupUi(this);

    disks = new UDisks2(this);
    connect(disks, &UDisks2::filesystemAdded, this, &dashboard::udisks2_filesystemAdded);
    connect(disks, &UDisks2::filesystemRemoved, this, &dashboard::udisks2_filesystemRemoved);
    connect(disks, &UDisks2::filesystemChanged, this, &dashboard::udisks2_filesystemChanged);
    connect(disks, &UDisks2::driveAdded, this, &dashboard::udisks2_driveAdded);
    connect(disks, &UDisks2::driveRemoved, this, &dashboard::udisks2_driveRemoved);
    connect(disks, &UDisks2::driveChanged, this, &dashboard::udisks2_driveChanged);
    connect(disks, &UDisks2::blockDeviceAdded, this, &dashboard::udisks2_blockAdded);
    connect(disks, &UDisks2::blockDeviceRemoved, this, &dashboard::udisks2_blockRemoved);
    connect(disks, &UDisks2::blockDeviceChanged, this, &dashboard::udisks2_blockChanged);

    ui->drives->addItems(disks->drives());
    for (int i = 0; i < disks->drives().count(); ++i) {
//        QListWidgetItem *p = new QListWidgetItem(disks->drives().at(i));
//        ui->horizontalLayout_4->setSizeConstraint(QLayout::SetFixedSize);
//        ui->widget->setLayout(ui->horizontalLayout_4);
//        p->setSizeHint(ui->widget->sizeHint());

//        ui->drives->addItem(p);
//        ui->drives->setItemWidget(p, ui->widget);
        ui->drives->item(i)->setIcon(QIcon(":/icons/hdd_b.svg"));
    }

    ui->blocks->addItems(disks->blockDevices());
    for (int i = 0; i < ui->blocks->count(); ++i) {
        ui->blocks->item(i)->setIcon(QIcon(":/icons/partition.svg"));
    }

    disks = new UDisks2(this);
    connect(disks, SIGNAL(blockDeviceAdded(QString)), this, SLOT(blockDevicesChanged()));
    connect(disks, SIGNAL(blockDeviceChanged(QString)), this, SLOT(blockDevicesChanged()));
    connect(disks, SIGNAL(blockDeviceRemoved(QString)), this, SLOT(blockDevicesChanged()));
    connect(disks, SIGNAL(filesystemAdded(QString)), this, SLOT(blockDevicesChanged()));

    m_upower = new UPower(this);
    foreach (Battery *bat, *m_upower->batteries()) {
        batteries.append(bat->path());
        ui->batteriesList->addItem(bat->model());
    }

    ui->batteriesList->setCurrentIndex(0);
    ui->mm->setText("Select a partition to ");
    ui->mount_2->setEnabled(0);
    ui->unmount_2->setEnabled(0);

    for (int i = 0; i < ui->pages->count(); i++) {
        ui->pages->removeWidget(ui->pages->widget(i));
    }

    sysinfo *infoo = new sysinfo();
    ui->pages->insertWidget(0, infoo);
    ui->pages->insertWidget(1, ui->pageDrives);
    ui->pages->insertWidget(2, ui->pageBattery);
    ResourcesPage *resourcePage = new ResourcesPage();
    ui->pages->insertWidget(3, resourcePage);

    ui->pages->setCurrentIndex(0);
}

dashboard::~dashboard()
{
    qDebug()<<"dashboard closing";
    delete disks;
    delete ui;
}
void dashboard::on_drives_currentTextChanged(const QString &currentText)
{
    auto dr = disks->drive(currentText);
    QStringList infos;
    infos
        << tr("Name : %1")       . arg(dr->toStringToSeperate(1))
        << tr("Size : %1")       . arg(dr->toStringToSeperate(2))
        << tr("Dev : %1")        . arg(dr->toStringToSeperate(3))
        << tr("Drive : %1")      . arg(dr->toStringToSeperate(4))
        << tr("Type : %1")       . arg(dr->toStringToSeperate(5))
        << tr("Id : %1 (Mhz)")   . arg(dr->toStringToSeperate(6))
        << tr("Read Only : %1")  . arg(dr->toStringToSeperate(7))
        << tr("Usage : %1")      . arg(dr->toStringToSeperate(8))
        << tr("Mount Point : %1"). arg(dr->toStringToSeperate(9))
        << tr("Overview : %1")   . arg(dr->toStringToSeperate(10));

    QStringListModel *systemInfoModel = new QStringListModel(infos);

    ui->info->setModel(systemInfoModel);
}

void dashboard::on_blocks_currentTextChanged(const QString &currentText)
{
    auto t = disks->blockDevice(ui->blocks->currentItem()->text())->fileSystem();
    if (t){
        auto block = disks->blockDevice(currentText);

        QString path = t->mountPoints().join("");

        QStringList infos;
        infos
            << tr("Name : %1")        . arg(block->toStringToSeperate(1))
            << tr("Size : %1")        . arg(block->toStringToSeperate(2))
            << tr("Dev : %1")         . arg(block->toStringToSeperate(3))
            << tr("Drive : %1")       . arg(block->toStringToSeperate(4))
            << tr("Type : %1")        . arg(block->toStringToSeperate(5))
            << tr("Id : %1 (Mhz)")    . arg(block->toStringToSeperate(6))
            << tr("Read Only : %1")   . arg(block->toStringToSeperate(7))
            << tr("Usage : %1")       . arg(block->toStringToSeperate(8))
            << tr("Mount Point : %1") . arg(path)
            << tr("Overview : %1")    . arg(getDriveInfo(path));

        QStringListModel *systemInfoModel = new QStringListModel(infos);

        ui->info->setModel(systemInfoModel);
    }
}

void dashboard::udisks2_blockAdded(const QString &node)
{
    ui->msgs->addItem("Connected block " + node);
    ui->blocks->addItem(node);
}

void dashboard::udisks2_blockRemoved(const QString &node)
{
    ui->msgs->addItem("Disconnected block " + node);
    foreach (auto item, ui->blocks->findItems(node, Qt::MatchExactly))
        delete item;
}

void dashboard::udisks2_blockChanged(const QString &node)
{
    ui->msgs->addItem("Block " + node + " changed");
}

void dashboard::udisks2_driveAdded(const QString &node)
{
    ui->msgs->addItem("Connected drive " + node);
    ui->drives->addItem(node);
}

void dashboard::udisks2_driveRemoved(const QString &node)
{
    ui->msgs->addItem("Disconnected drive " + node);
    auto items = ui->drives->findItems(node, Qt::MatchExactly);
    foreach (auto item, items)
        delete item;
}

void dashboard::udisks2_driveChanged(const QString &node)
{
    ui->msgs->addItem("Drive " + node + " changed");
}

void dashboard::udisks2_filesystemAdded(const QString &node)
{
    ui->msgs->addItem("Block " + node + " got a filesystem");
}

void dashboard::udisks2_filesystemRemoved(const QString &node)
{
    ui->msgs->addItem("Block " + node + " lost a filesystem");
}

void dashboard::udisks2_filesystemChanged(const QString &node)
{
    ui->msgs->addItem("Block " + node + " changed its filesystem");
}

void dashboard::on_mount_2_clicked()
{
    if (ui->blocks->currentItem()->text().isNull())
        return;

    auto fs = disks->blockDevice(ui->blocks->currentItem()->text())->fileSystem();

    if (fs)
        messageEngine("Directory '" + fs->mount() + "' mounted.", "Info");
}

void dashboard::on_unmount_2_clicked()
{
    if (!ui->blocks->currentItem())
        return;
    auto fs = disks->blockDevice(ui->blocks->currentItem()->text())->fileSystem();
    if (fs) {
        fs->unmount();
        messageEngine("Partition '" + fs->name + "' unmounted.", "Info");
    }
}

QString dashboard::getDriveInfo(QString path)
{
    struct statfs info;
    statfs(path.toLocal8Bit(), &info);

    if(info.f_blocks == 0) return "";

    return QString("%1  /  %2  (%3%)").arg(formatSize((qint64) (info.f_blocks - info.f_bavail)*info.f_bsize))
                       .arg(formatSize((qint64) info.f_blocks*info.f_bsize))
                       .arg((info.f_blocks - info.f_bavail)*100/info.f_blocks);
}

void dashboard::on_blocks_itemSelectionChanged()
{
    ui->mm->setText("Selected partition");
    ui->mount_2->setEnabled(1);
    ui->unmount_2->setEnabled(1);
    ui->drives->clearSelection();
}

void dashboard::on_drives_itemSelectionChanged()
{
    ui->mm->setText("Select a partition to ");
    ui->mount_2->setEnabled(0);
    ui->unmount_2->setEnabled(0);
    ui->blocks->clearSelection();
}

void dashboard::on_batteriesList_currentIndexChanged(int index)
{
    m_model = m_upower->batteries()->value(batteries.at(index));

    ui->lvlProgress->setValue(m_model->percentage());
    ui->capacityProgress->setValue(m_model->capacity());

    QTime time;
    double energy = m_model->energy();
    double energyEmpty = m_model->energyEmpty();
    double energyFull = m_model->energyFull();
    double rate = m_model->energyRate();
    int addSeconds = 0;
    switch( m_model->state() ) {
        case Battery::FullyCharged:
            ui->statusEdit->setText( tr( "Full" ) );
            ui->timerLblEdit->setText( tr( "Full" ) );
            break;
        case Battery::Discharging:
            addSeconds = m_model->toEmpty();
            addSeconds = rate != 0 && addSeconds == 0 ? ( energy - energyEmpty ) * 4 / rate: addSeconds;
            ui->statusEdit->setText( tr( "Discharging" ) );
            ui->timerLblEdit->setText( tr( "Discharged in : " ) );
            break;
        case Battery::Charging:
            addSeconds = m_model->toFull();
            addSeconds = rate != 0 && addSeconds == 0 ? ( energyFull - energy ) * 4 / rate : addSeconds;
            ui->statusEdit->setText( tr( "Charging" ) );
            ui->timerLblEdit->setText( tr( "Charged in : " ) );
            break;
        default:
            ui->statusEdit->setText( tr( "No Battery" ) );
            ui->timerLblEdit->setText( tr( "No Battery" ) );
            break;
    }

    time = time.addSecs(addSeconds);
    ui->timerEdit->setTime(time);

    QList<QTreeWidgetItem*> listItems;
    ui->propertiesList->clear();
    const QMetaObject *metaObject = m_model->metaObject();
    int count = metaObject->propertyCount();
    for( int i = 0; i < count; ++i ) {
        QMetaProperty metaProperty = metaObject->property(i);
        QString name = metaProperty.name();
        if( name != "objectName" ) {
            QVariant value = m_model->property(name.toUtf8());
            if( value.type() == QVariant::Double ) {
                listItems << new QTreeWidgetItem(ui->propertiesList, QStringList() << name << QString( "%1" ).number( value.toDouble() ), QTreeWidgetItem::Type );
            } else {
                listItems << new QTreeWidgetItem(ui->propertiesList, QStringList() << name << value.toString(), QTreeWidgetItem::Type );
            }
        }
    }
    ui->propertiesList->addTopLevelItems(listItems);
    ui->propertiesList->resizeColumnToContents(0);
    ui->propertiesList->resizeColumnToContents(1);
}

void dashboard::on_refresh_clicked()
{
    on_batteriesList_currentIndexChanged(ui->batteriesList->currentIndex());
}

void dashboard::pageClick(QPushButton *btn, int i, QString title)
{
    // all button checked false
    for (QPushButton *b : ui->shortcut->findChildren<QPushButton*>())
        b->setChecked(false);
    btn->setChecked(true);
    ui->selectedsection->setText(title);
    ui->pages->setCurrentIndex(i);
}

void dashboard::on_Bresource_clicked()
{
    pageClick(ui->Bresource, 3, tr("Resource"));
}

void dashboard::on_Bdrives_clicked()
{
    pageClick(ui->Bdrives, 1, tr("Drives"));
}

void dashboard::on_Bbattery_clicked()
{
    pageClick(ui->Bbattery, 2, tr("Battery"));
}

void dashboard::on_Bgeneral_clicked()
{
    pageClick(ui->Bgeneral, 0, tr("General"));
}


