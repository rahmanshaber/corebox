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


Start::Start(QWidget *parent) :QWidget(parent),ui(new Ui::Start)
{
    ui->setupUi(this);

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
        loadRecent();
    }

    loadSession();
}

Start::~Start()
{
    delete ui;
}

// ======== Core Apps ==========
void Start::on_appCollect_itemDoubleClicked(QListWidgetItem *item) // open SpeedDial on CoreApps
{
    CoreBox *cBox = static_cast<CoreBox*>(qApp->activeWindow());
    cBox->tabEngine(nameToInt(item->text()));
}
// =============================


// ======== Speed Dial ==========
void Start::on_speedDialB_itemDoubleClicked(QListWidgetItem *item) // open SpeedDial on doubleclick
{
    BookmarkManage bk;
    // Function from globalfunctions.cpp openAppEngine(..)
    openAppEngine(bk.bookmarkPath("Speed Dial", item->text()));
}

void Start::loadSpeedDial() // populate SpeedDial list
{
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

void Start::on_recentActivitesL_itemDoubleClicked(QTreeWidgetItem *item, int column) // Open Recent activity on double click
{
    if (!item->text(column).contains("\t\t\t"))
        return;

    QStringList s = item->text(column).split("\t\t\t");
    // Function from globalfunctions.cpp
    openAppEngine(s.at(1));
}

void Start::loadRecent() // populate RecentActivity list
{
    ui->recentActivitesL->clear();
    QSettings recentActivity(QDir::homePath() + "/.config/coreBox/RecentActivity", QSettings::IniFormat);
    QStringList topLevel = recentActivity.childGroups();
    sortDate(topLevel);
    foreach (QString group, topLevel) {
        QTreeWidgetItem *topTree = new QTreeWidgetItem();
        QString groupL = sentDateText(group);
        topTree->setText(0, groupL);
        recentActivity.beginGroup(group);
        QStringList keys = recentActivity.childKeys();
        sortTime(keys);
        foreach (QString key, keys) {
            QTreeWidgetItem *child = new QTreeWidgetItem();
            QString value = recentActivity.value(key).toString();
            child->setText(0, value);
            child->setIcon(0, geticon(value.split("\t\t\t").at(1)));
            topTree->addChild(child);
        }
        recentActivity.endGroup();
        ui->recentActivitesL->insertTopLevelItem(0, topTree);
    }

    if (topLevel.count())
        (ui->recentActivitesL->setExpanded(ui->recentActivitesL->model()->index(0, 0), true));
}

// =================================

void Start::loadSession()
{
    ui->sessionsList->clear();
    QSettings session(QDir::homePath() + "/.config/coreBox/Sessions", QSettings::IniFormat);
    QStringList topLevel = session.childGroups();
    sortDateTime(topLevel);
    foreach (QString group, topLevel) {
        QTreeWidgetItem *topTree = new QTreeWidgetItem();
        QString groupL = sentDateText(group.split(" - ").at(1));
        topTree->setText(0, groupL);
        session.beginGroup(group);
        QStringList keys = session.childKeys();
        sortList(keys);
        foreach (QString key, keys) {
            QTreeWidgetItem *child = new QTreeWidgetItem();
            QString value = session.value(key).toString();
            child->setText(0, key + "\t\t\t" + value);
            child->setIcon(0, geticon(value));
            topTree->addChild(child);
        }
        session.endGroup();
        ui->sessionsList->insertTopLevelItem(0, topTree);
    }

    if (topLevel.count())
        (ui->sessionsList->setExpanded(ui->sessionsList->model()->index(0, 0), true));

}

void Start::loadsettings() // load settings
{
    // Check is recent disabled or not
    if (sm.getDisableRecent()) {
        ui->recentActivites->setVisible(0);
        ui->recentActivitesL->clear();
        ui->pages->setCurrentIndex(0);

    } else {
        ui->recentActivites->setVisible(1);
        loadRecent();
    }
}

void Start::pageClick(QPushButton *btn, int i)
{
    // all button checked false
    for (QPushButton *b : ui->pageButtons->findChildren<QPushButton*>())
        b->setChecked(false);
    btn->setChecked(true);
    ui->pages->slideInIdx(i);
}

void Start::on_coreApps_clicked()
{
    pageClick(ui->coreApps, 0);
}

void Start::on_speedDial_clicked()
{
    pageClick(ui->speedDial, 1);
}

void Start::on_recentActivites_clicked()
{
    pageClick(ui->recentActivites, 2);
}

void Start::on_savedSession_clicked()
{
    pageClick(ui->savedSession, 3);
}

void Start::reload()
{
    loadSpeedDial();
    loadSession();
    loadsettings();
    if (!sm.getDisableRecent())
        loadRecent();
    else on_coreApps_clicked();
}


