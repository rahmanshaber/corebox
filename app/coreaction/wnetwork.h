#ifndef WNETWORK_H
#define WNETWORK_H

#include <QWidget>
#include "dashboard/Managers/info_manager.h"

namespace Ui {
class wNetwork;
}

class wNetwork : public QWidget
{
    Q_OBJECT

public:
    explicit wNetwork(QWidget *parent = 0);
    ~wNetwork();

private:
    Ui::wNetwork *ui;

    InfoManager *info;

    void checkNetwork();
};

#endif // WNETWORK_H
