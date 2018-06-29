#ifndef WBATTERY_H
#define WBATTERY_H

#include <QWidget>

#include "dashboard/upower.h"

namespace Ui {
class wBattery;
}

class wBattery : public QWidget
{
    Q_OBJECT

public:
    explicit wBattery(QWidget *parent = 0);
    ~wBattery();

private:
    Ui::wBattery *ui;
    UPower *u;

    void batteryCheck();
};

#endif // WBATTERY_H
