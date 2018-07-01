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

#include "wnetwork.h"
#include "ui_wnetwork.h"


wNetwork::wNetwork(QWidget *parent) :QWidget(parent),ui(new Ui::wNetwork),
    info(InfoManager::ins())
{
    ui->setupUi(this);
    checkNetwork();
    QTimer *timer = new QTimer();
    timer->start(1000);
    connect(timer, &QTimer::timeout, this, &wNetwork::checkNetwork);
}

wNetwork::~wNetwork()
{
    delete ui;
}

void wNetwork::checkNetwork()
{
    static quint64 l_RXbytes = info->getRXbytes();
    static quint64 l_TXbytes = info->getTXbytes();
    static quint64 max_RXbytes = 1L << 20; // 1 MEBI
    static quint64 max_TXbytes = 1L << 20; // 1 MEBI

    quint64 RXbytes = info->getRXbytes();
    quint64 TXbytes = info->getTXbytes();

    quint64 d_RXbytes = (RXbytes - l_RXbytes);
    quint64 d_TXbytes = (TXbytes - l_TXbytes);

    QString downText = formatSize(d_RXbytes);
    QString upText = formatSize(d_TXbytes);

    ui->dspeed->setText(downText);
    ui->uspeed->setText(upText);

    max_RXbytes = qMax(max_RXbytes, d_RXbytes);
    max_TXbytes = qMax(max_TXbytes, d_TXbytes);

    l_RXbytes = RXbytes;
    l_TXbytes = TXbytes;
}
