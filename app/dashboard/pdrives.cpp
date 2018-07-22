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

#include "pdrives.h"
#include "ui_pdrives.h"


pDrives::pDrives(QWidget *parent) :QWidget(parent),ui(new Ui::pDrives)
{
    ui->setupUi(this);
    setupDisksPage();
    ui->mm->setText("Select a Partition to");
    addDropShadow(ui->alldrives,40);
    addDropShadow(ui->logs,40);
    addDropShadow(ui->partiton,40);
    addDropShadow(ui->properties,40);
}

pDrives::~pDrives()
{
    delete ui;
    disks->deleteLater();
}

void pDrives::reload()
{

}

QString pDrives::getDriveInfo(const QString path)
{
    double t = QStorageInfo(path).bytesTotal();
    double f = QStorageInfo(path).bytesFree();

    // Function from globalfunctions.cpp
    return QString("%1  /  %2  (%3%)").arg(formatSize(f)).arg(formatSize(t)).arg((t - f)*100/t);
}

void pDrives::setupDisksPage()
{
    disks = new UDisks2(this);
    connect(disks, &UDisks2::filesystemAdded, this, &pDrives::udisks2_filesystemAdded);
    connect(disks, &UDisks2::filesystemRemoved, this, &pDrives::udisks2_filesystemRemoved);
    connect(disks, &UDisks2::filesystemChanged, this, &pDrives::udisks2_filesystemChanged);
    connect(disks, &UDisks2::driveAdded, this, &pDrives::udisks2_driveAdded);
    connect(disks, &UDisks2::driveRemoved, this, &pDrives::udisks2_driveRemoved);
    connect(disks, &UDisks2::driveChanged, this, &pDrives::udisks2_driveChanged);
    connect(disks, &UDisks2::blockDeviceAdded, this, &pDrives::udisks2_blockAdded);
    connect(disks, &UDisks2::blockDeviceRemoved, this, &pDrives::udisks2_blockRemoved);
    connect(disks, &UDisks2::blockDeviceChanged, this, &pDrives::udisks2_blockChanged);

    ui->drives->addItems(disks->drives());
    for (int i = 0; i < disks->drives().count(); ++i) {
//        QListWidgetItem *p = new QListWidgetItem(disks->drives().at(i));
//        ui->horizontalLayout_4->setSizeConstraint(QLayout::SetFixedSize);
//        ui->widget->setLayout(ui->horizontalLayout_4);
//        p->setSizeHint(ui->widget->sizeHint());

//        ui->drives->addItem(p);
//        ui->drives->setItemWidget(p, ui->widget);
        ui->drives->item(i)->setIcon(QIcon(":/icons/drive_hdd.svg"));
    }

//    QStringList s = disks->blockDevices();
//    QStringList l = s.filter(QRegularExpression(QStringLiteral("[a-z]{3}")));
//    ui->blocks->addItems(l);

    ui->blocks->addItems(disks->blockDevices());
    for (int i = 0; i < ui->blocks->count(); ++i) {
           ui->blocks->item(i)->setIcon(QIcon(":/icons/drive.svg"));
    }

//    disks = new UDisks2(this);
//    connect(disks, SIGNAL(blockDeviceAdded(QString)), this, SLOT(blockDevicesChanged()));
//    connect(disks, SIGNAL(blockDeviceChanged(QString)), this, SLOT(blockDevicesChanged()));
//    connect(disks, SIGNAL(blockDeviceRemoved(QString)), this, SLOT(blockDevicesChanged()));
//    connect(disks, SIGNAL(filesystemAdded(QString)), this, SLOT(blockDevicesChanged()));

    ui->mount_2->setEnabled(0);
    ui->unmount_2->setEnabled(0);
}

void pDrives::on_drives_currentTextChanged(const QString &currentText)
{
    auto dr = disks->drive(currentText);

    QStringList left;
    QStringList right;
    left << "Name" << "Size" << "Dev" << "Drive" << "Type" << "Id" << "Read Only"
         << "Usage" << "Mount Point" << "Overview";
    right << dr->toStringToSeperate(1) << dr->toStringToSeperate(2) << dr->toStringToSeperate(3)
          << dr->toStringToSeperate(4) << dr->toStringToSeperate(5) << dr->toStringToSeperate(6)
          << dr->toStringToSeperate(7) << dr->toStringToSeperate(8) << dr->toStringToSeperate(9)
          << dr->toStringToSeperate(10);


    QStringListModel *systemInfoModel = new QStringListModel(fStringList(left, right, ui->info->font()));

    ui->info->setModel(systemInfoModel);
}

void pDrives::on_blocks_currentTextChanged(const QString &currentText)
{
    auto t = disks->blockDevice(ui->blocks->currentItem()->text())->fileSystem();
    if (t){
        auto block = disks->blockDevice(currentText);

        QString path = t->mountPoints().join("");

        QStringList left;
        QStringList right;
        left << "Name" << "Size" << "Dev" << "Drive" << "Type" << "Id" << "Read Only"
             << "Usage" << "Mount Point" << "Overview";
        right << block->toStringToSeperate(1) << block->toStringToSeperate(2) << block->toStringToSeperate(3)
              << block->toStringToSeperate(4) << block->toStringToSeperate(5) << block->toStringToSeperate(6)
              << block->toStringToSeperate(7) << block->toStringToSeperate(8) << path
              << getDriveInfo(path);

        QStringListModel *systemInfoModel = new QStringListModel(fStringList(left, right, ui->info->font()));

        ui->info->setModel(systemInfoModel);
    }
}

void pDrives::udisks2_blockAdded(const QString &node)
{
    ui->msgs->addItem("Connected block " + node);
    ui->blocks->addItem(node);
}

void pDrives::udisks2_blockRemoved(const QString &node)
{
    ui->msgs->addItem("Disconnected block " + node);
    foreach (auto item, ui->blocks->findItems(node, Qt::MatchExactly))
        delete item;
}

void pDrives::udisks2_blockChanged(const QString &node)
{
    ui->msgs->addItem("Block " + node + " changed");
}

void pDrives::udisks2_driveAdded(const QString &node)
{
    ui->msgs->addItem("Connected drive " + node);
    ui->drives->addItem(node);
}

void pDrives::udisks2_driveRemoved(const QString &node)
{
    ui->msgs->addItem("Disconnected drive " + node);
    auto items = ui->drives->findItems(node, Qt::MatchExactly);
    foreach (auto item, items)
        delete item;
}

void pDrives::udisks2_driveChanged(const QString &node)
{
    ui->msgs->addItem("Drive " + node + " changed");
}

void pDrives::udisks2_filesystemAdded(const QString &node)
{
    ui->msgs->addItem("Block " + node + " got a filesystem");
}

void pDrives::udisks2_filesystemRemoved(const QString &node)
{
    ui->msgs->addItem("Block " + node + " lost a filesystem");
}

void pDrives::udisks2_filesystemChanged(const QString &node)
{
    ui->msgs->addItem("Block " + node + " changed its filesystem");
}

void pDrives::on_mount_2_clicked()
{
    if (ui->blocks->currentItem()->text().isNull())
        return;

    auto fs = disks->blockDevice(ui->blocks->currentItem()->text())->fileSystem();

    if (fs)
        messageEngine("Directory '" + fs->mount() + "' mounted.", MessageType::Info);
}

void pDrives::on_unmount_2_clicked()
{
    if (!ui->blocks->currentItem())
        return;
    auto fs = disks->blockDevice(ui->blocks->currentItem()->text())->fileSystem();
    if (fs) {
        fs->unmount();
        messageEngine("Partition '" + fs->name + "' unmounted.", MessageType::Info);
    }
}

void pDrives::on_blocks_itemSelectionChanged()
{
    ui->mm->setText("Selected partition");
    ui->mount_2->setEnabled(1);
    ui->unmount_2->setEnabled(1);
    ui->drives->clearSelection();
}

void pDrives::on_drives_itemSelectionChanged()
{
    ui->mm->setText("Select a partition to ");
    ui->mount_2->setEnabled(0);
    ui->unmount_2->setEnabled(0);
    ui->blocks->clearSelection();
}
