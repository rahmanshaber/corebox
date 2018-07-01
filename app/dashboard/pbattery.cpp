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

#include "pbattery.h"
#include "ui_pbattery.h"


pBattery::pBattery(QWidget *parent) :QWidget(parent),ui(new Ui::pBattery)
{
    ui->setupUi(this);
    setupBatteryPage();
}

pBattery::~pBattery()
{
    delete ui;
    m_upower->deleteLater();
    m_model->deleteLater();
}

void pBattery::reload()
{
    on_batteriesList_currentIndexChanged(ui->batteriesList->currentIndex());
}

void pBattery::setupBatteryPage()
{
    m_upower = new UPower(this);
    foreach (Battery *bat, *m_upower->batteries()) {
        batteries.append(bat->path());
        ui->batteriesList->addItem(bat->model());
    }

    ui->batteriesList->setCurrentIndex(0);
}

void pBattery::on_batteriesList_currentIndexChanged(int index)
{
    m_model = m_upower->batteries()->value(batteries.at(index));

    ui->lvlProgress->setValue(m_model->percentage());
    ui->capacityProgress->setValue(m_model->capacity());

    QTime time(0, 0, 0, 0);
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

    // Funstion from globalfunctions.cpp fStringList
    QStringListModel *infoModel = new QStringListModel(fStringList(left, right, ui->batProperties->font()));

    ui->batProperties->setModel(infoModel);
}

void pBattery::on_refresh_clicked()
{
    reload();
}
