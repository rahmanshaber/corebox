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

Start::Start(QWidget *parent) :QWidget(parent),ui(new Ui::Start)
{
    ui->setupUi(this);

    ui->recents->setFocusPolicy(Qt::NoFocus);
    loadsettings();

    on_coreApps_clicked();
    reload();
}

Start::~Start()
{
    delete ui;
}

void Start::loadsettings()
{
//    ui->recentSection->setVisible(sm.getDisableRecent());
}

void Start::on_recents_itemDoubleClicked(QTableWidgetItem *item)
{
    openAppEngine(ui->recents->item(item->row(),1)->text());
}

void Start::on_appCollect_itemDoubleClicked(QListWidgetItem *item)
{
    CoreBox *cBox = qobject_cast<CoreBox*>(qApp->activeWindow());
    cBox->tabEngine(nameToInt(item->text()));
}

void Start::reload()
{
    //reload speeddial bookmarks
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

    //reload recent activities list
    if (sm.getDisableRecent() == true){

        ui->recents->clear();
        sm.cSetting->beginGroup("Recent");

        QTableWidgetItem *item;
        QStringList sList = sm.cSetting->childKeys();
        QStringList r;
        if (sList.count() > 30) {
            int extra = sList.count() - 30;
            for (int i = 0; i < extra; ++i) {
                sList.removeAt(sList.indexOf(QString::number(i)));
            }
        }

        ui->recents->setColumnCount(3);
        ui->recents->setRowCount(sList.count());

        for (int i = 0; i < sList.count(); ++i) {
            r = sm.cSetting->value(sList.at(i)).toString().split("$$$");
            item = new QTableWidgetItem(r.count() == 3 ? appsIconPath(r.at(0)) : appsIconPath(r.at(2)), r.at(0));
            ui->recents->setItem(i, 0, item);
            ui->recents->setItem(i, 1, new QTableWidgetItem(r.at(1)));
            ui->recents->setItem(i, 2, new QTableWidgetItem(r.at(2)));
        }
        ui->recents->sortByColumn(2, Qt::DescendingOrder);

        sm.cSetting->endGroup();

        ui->recents->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    }
}


void Start::on_speedDialB_itemDoubleClicked(QListWidgetItem *item)
{
    BookmarkManage bk;
    openAppEngine(bk.bookmarkPath("Speed Dial",item->text()));
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
