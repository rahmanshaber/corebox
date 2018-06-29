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

#include "start.h"
#include "ui_start.h"

#include <QTableWidgetItem>
#include <QFont>
#include <QFileSystemWatcher>
#include <QDebug>
#include <QCollator>

#include "corebox/corebox.h"
#include "bookmarks/bookmarkmanage.h"
#include "corebox/globalfunctions.h"


Start::Start(QWidget *parent) :QWidget(parent),ui(new Ui::Start)
{
    ui->setupUi(this);
    //setWindowOpacity(0.95);
    //setAttribute(Qt::WA_TranslucentBackground);
    QFileSystemWatcher *watcher = new QFileSystemWatcher();

    // Get recent activity enabled or not
    isRecentEnable = !sm.getDisableRecent();

    // Configure Settings
    loadsettings();

    // Set coreapps page as active page
    on_coreApps_clicked();

    // Configure Speed Dial
    loadSpeedDial();

    // Configure Recent Activity
    if (!sm.getDisableRecent()) {
        QString raFile = QDir::homePath() + "/.config/coreBox/RecentActivity";
        QFile file(raFile);
        if (!file.exists()) {
            // You can get error
            // Need a check here
            file.open(QIODevice::ReadWrite | QIODevice::Text);
            file.close();
        }
        watcher->addPath(raFile);
        loadRecent();
    }

    watcher->addPath(QDir::homePath() + "/.config/coreBox/coreBox.conf");
    watcher->addPath(QDir::homePath() + "/.config/coreBox/CoreBoxBook");

//    connect(watcher, &QFileSystemWatcher::fileChanged, [this](const QString &path) {
//        qDebug() << path;
//        if (QFileInfo(path).fileName() == "RecentActivity") {
//            loadRecent();
//        } else if (QFileInfo(path).fileName() == "CoreBoxBook") {
//            loadSpeedDial();
//        } else if (QFileInfo(path).fileName() == "coreBox.conf") {
//            isRecentEnable = !sm.getDisableRecent();
//            loadsettings();
//        }
//    });
}

Start::~Start()
{
    delete ui;
}

// ======== Core Apps ==========
// Open CoreApps on double click
void Start::on_appCollect_itemDoubleClicked(QListWidgetItem *item)
{
    CoreBox *cBox = static_cast<CoreBox*>(qApp->activeWindow());
    cBox->tabEngine(nameToInt(item->text()));
}
// =============================


// ======== Speed Dial ==========
// Open Speed Dial on Double click
void Start::on_speedDialB_itemDoubleClicked(QListWidgetItem *item)
{
    BookmarkManage bk;
    openAppEngine(bk.bookmarkPath("Speed Dial",item->text()));
}

// Load Speed Dial
void Start::loadSpeedDial() {
    ui->speedDialB->clear();
    BookmarkManage bk;
    QStringList list = bk.getBookNames("Speed Dial");
    for (int i = 0; i < list.count(); ++i) {
        if (i == 15) {
            return;
        } else {
            ui->speedDialB->addItem(new QListWidgetItem(geticon(bk.bookmarkPath("Speed Dial", list.at(i))), list.at(i)));
        }
    }
}
// =============================


// ========== Recent activity ===========
// Open Recent activity on double click
void Start::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    QStringList s = item->text(column).split("\t\t\t");
    openAppEngine(s.at(1));
}

// Load Recent Activity
void Start::loadRecent()
{
    ui->treeWidget->clear();
    QSettings recentActivity(QDir::homePath() + "/.config/coreBox/RecentActivity", QSettings::IniFormat);
    QStringList topLevel = recentActivity.childGroups();
    foreach (QString group, topLevel) {
        QTreeWidgetItem *topTree = new QTreeWidgetItem();
        QString groupL = sentDateText(group);
        topTree->setText(0, groupL);
        recentActivity.beginGroup(group);
        QStringList keys = recentActivity.childKeys();
        QCollator sort;
        sort.setNumericMode(true);
        std::sort(keys.begin(), keys.end(), sort);
        foreach (QString key, keys) {
            QTreeWidgetItem *child = new QTreeWidgetItem();
            QString value = recentActivity.value(key).toString();
            child->setText(0, value);
            child->setIcon(0, geticon(value.split("\t\t\t").at(1)));
            topTree->addChild(child);
        }
        recentActivity.endGroup();
        ui->treeWidget->insertTopLevelItem(0, topTree);
    }

    if (topLevel.count())
        (ui->treeWidget->setExpanded(ui->treeWidget->model()->index(0, 0), true));
}
// =================================


// Load Settings
void Start::loadsettings()
{
    // Check is recent disabled or not
    if (sm.getDisableRecent()) {
        //ui->pages->removeWidget(ui->precents);
        ui->recentActivites->setVisible(0);
        ui->treeWidget->clear();
        ui->pages->setCurrentIndex(0);

    } else {
        //if (ui->pages->count() == 3) {
        //    ui->recentActivites->setVisible(1);
        //} else {
            ui->recentActivites->setVisible(1);
            loadRecent();
        //    ui->pages->insertWidget(2, ui->precents);
        //}
    }
}

// Don't delete
void Start::paintEvent(QPaintEvent *event)
{
//    QRgb _blend(qRgba(0,0,0,0xff));
//    QColor color(_blend);
//    color.setAlphaF(0.75);
//    _blend = color.rgba();

//    QPainter paint(this);
//    paint.setOpacity(0.8);

//    const auto rects = (event->region() & contentsRect()).rects();
//    //qDebug() << rects;
//    for (const QRect &rect : rects)
//    {
//        QColor col(QColor::fromRgb(255,255,255));
//        col.setAlpha(qAlpha(_blend));
//        paint.save();
//        paint.setCompositionMode(QPainter::CompositionMode_Source);
//        paint.fillRect(rect, col);
//        paint.restore();
//    }
}

void Start::on_coreApps_clicked()
{
    ui->pages->setCurrentIndex(0);
    ui->coreApps->setChecked(1);
    ui->speedDial->setChecked(0);
    ui->recentActivites->setChecked(0);
}

void Start::on_speedDial_clicked()
{
    ui->pages->setCurrentIndex(1);
    ui->speedDial->setChecked(1);
    ui->coreApps->setChecked(0);
    ui->recentActivites->setChecked(0);
}

void Start::on_recentActivites_clicked()
{
    ui->pages->setCurrentIndex(2);
    ui->recentActivites->setChecked(1);
    ui->speedDial->setChecked(0);
    ui->coreApps->setChecked(0);
}

void Start::reload()
{
    loadSpeedDial();
    loadsettings();
    if (!sm.getDisableRecent())
        loadRecent();
    else on_coreApps_clicked();
}
