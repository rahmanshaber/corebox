#ifndef PBATTERY_H
#define PBATTERY_H

#include <QWidget>

#include "upower.h"
#include "battery.h"

namespace Ui {
class pBattery;
}

class pBattery : public QWidget
{
    Q_OBJECT

public:
    explicit pBattery(QWidget *parent = 0);
    ~pBattery();

    void reload();

private slots:
    void on_batteriesList_currentIndexChanged(int index);

    void on_refresh_clicked();

private:
    Ui::pBattery *ui;
    UPower *m_upower;
    Battery *m_model;
    QStringList batteries;

    void setupBatteryPage();
};

#endif // PBATTERY_H
