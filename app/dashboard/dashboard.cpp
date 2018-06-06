
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
#include <QSize>


dashboard::dashboard(QWidget *parent) :QWidget(parent),ui(new Ui::dashboard)
{
    ui->setupUi(this);

    setupBatteryPage();
    setupDisksPage();
    setupDisplayPage();

    //fixed
    //for unknown bug ,first remove all page then add each
//    for (int i = 0; i < ui->pages->count(); i++) {
//        ui->pages->removeWidget(ui->pages->widget(i));
//    }
    sysinfo *infoo = new sysinfo();
    QHBoxLayout *hl = new QHBoxLayout();
    hl->setContentsMargins(0, 0, 0, 0);
    hl->addWidget(infoo);
    ui->pages->widget(0)->setLayout(hl);

    ResourcesPage *resourcePage = new ResourcesPage();
    QHBoxLayout *hl2 = new QHBoxLayout();
    hl2->setContentsMargins(0, 0, 0, 0);
    hl2->addWidget(resourcePage);
    ui->pages->widget(3)->setLayout(hl2);


    ui->pages->setCurrentIndex(0);
    ui->mm->setText("Select a partition to ");
}

dashboard::~dashboard()
{
    delete disks;
    delete ui;
}

void dashboard::setupBatteryPage()
{
    m_upower = new UPower(this);
    foreach (Battery *bat, *m_upower->batteries()) {
        batteries.append(bat->path());
        ui->batteriesList->addItem(bat->model());
    }

    ui->batteriesList->setCurrentIndex(0);
}

void dashboard::setupDisksPage()
{
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
        ui->drives->item(i)->setIcon(QIcon(":/icons/drive_hdd_w.svg"));
    }

//    QStringList s = disks->blockDevices();
//    QStringList l = s.filter(QRegularExpression(QStringLiteral("[a-z]{3}")));
//    ui->blocks->addItems(l);

    ui->blocks->addItems(disks->blockDevices());
    for (int i = 0; i < ui->blocks->count(); ++i) {
           ui->blocks->item(i)->setIcon(QIcon(":/icons/drive_w.svg"));
    }

//    disks = new UDisks2(this);
//    connect(disks, SIGNAL(blockDeviceAdded(QString)), this, SLOT(blockDevicesChanged()));
//    connect(disks, SIGNAL(blockDeviceChanged(QString)), this, SLOT(blockDevicesChanged()));
//    connect(disks, SIGNAL(blockDeviceRemoved(QString)), this, SLOT(blockDevicesChanged()));
//    connect(disks, SIGNAL(filesystemAdded(QString)), this, SLOT(blockDevicesChanged()));

    ui->mount_2->setEnabled(0);
    ui->unmount_2->setEnabled(0);
}


void dashboard::on_drives_currentTextChanged(const QString &currentText)
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

void dashboard::on_blocks_currentTextChanged(const QString &currentText)
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

QString dashboard::getDriveInfo(const QString path)
{
    double t = QStorageInfo(path).bytesTotal();
    double f = QStorageInfo(path).bytesFree();

    return QString("%1  /  %2  (%3%)").arg(formatSize(f))
                       .arg(formatSize(t))
                       .arg((t - f)*100/t);
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
            ui->statusEdit->setText( tr( "Full Charged" ) );
            ui->timerLblEdit->setText( tr( "Full Charged" ) );
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

    QStringList left;
    QStringList right;

    const QMetaObject *metaObject = m_model->metaObject();
    int count = metaObject->propertyCount();
    for( int i = 0; i < count; ++i ) {
        QMetaProperty metaProperty = metaObject->property(i);
        QString name = metaProperty.name();
        if( name != "objectName" ) {
            QVariant value = m_model->property(name.toUtf8());            

            left << name;

            if( value.type() == QVariant::Double ) {
                right << QString::number(value.toDouble());
            } else {
                right << value.toString();
            }
        }
    }

    QStringListModel *infoModel = new QStringListModel(fStringList(left, right, ui->batProperties->font()));

    ui->batProperties->setModel(infoModel);
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

void dashboard::on_Bdisplay_clicked()
{
    pageClick(ui->Bdisplay, 4, tr("Display"));
}

void dashboard::setupDisplayPage()
{
    QStringList left;
    left << "Name        " << "Size        " << "Manufacturer        " << "Model      "
         << "Serial Number         " << "Refresh Rate        " << "Actual Resolution     "
         << "Set Resolution        " << "Physical Dots Per Inch "
         << "Physical Size        " << "Primary Orientation   ";

    for (int i = 0; i < qApp->screens().count(); i++) {

        QSize s = qApp->screens()[i]->size();
        QString size(tr("(%1,%2)").arg(s.width()).arg(s.height()));

        QSize a = qApp->screens()[i]->availableVirtualSize();
        QString AvailableVS(tr("(%1,%2)").arg(a.width()).arg(a.height()));

        QRect g = qApp->screens()[i]->geometry();
        QString Geometry(tr("(%1,%2)").arg(g.width()).arg(g.height()));

        QSizeF py = qApp->screens()[i]->physicalSize();
        QString PhysicalSize(tr("(%1,%2)").arg(py.width()).arg(py.height()));

        QStringList right;

        QListView *p = new QListView();
        QWidget *w = new QWidget();
        QFont fl ("Cantarell", 14, QFont::Normal);
        QFont fp ("Ubuntu Mono", 11, QFont::Normal);
        QLabel *l = new QLabel("Screen : " + QString::number(i+1));
        QVBoxLayout *v = new QVBoxLayout();
        QHBoxLayout *h = new QHBoxLayout();


        right << qApp->screens()[i]->name() << size << qApp->screens()[i]->manufacturer()
              << qApp->screens()[i]->model() << qApp->screens()[i]->serialNumber()
              << QString::number(qApp->screens()[i]->refreshRate()) << AvailableVS << Geometry
              << QString::number(qApp->screens()[i]->physicalDotsPerInch()) << PhysicalSize
              << QString::number(qApp->screens()[i]->primaryOrientation());

        QStringListModel *systemInfoModel = new QStringListModel(fStringList(left, right, p->font()));

        p->setModel(systemInfoModel);

        l->setFont(fl);
        p->setFont(fp);

        h->addWidget(p);
        h->setContentsMargins(1, 1, 1, 5);

        v->addWidget(l);
        v->addLayout(h);

        w->setLayout(v);

        p->setStyleSheet("QWidget{background-color: #1B252F; ;color: #ffffff;padding: 5px 5px 5px 5px;}");
        w->setStyleSheet("QWidget{background-color:#1F2B38;color:#ffffff;}");

        ui->list->addWidget(w);

        right.clear();
    }
}

void dashboard::reload()
{
    setupDisksPage();
    on_batteriesList_currentIndexChanged(ui->batteriesList->currentIndex());
}
