#ifndef WSYSTEM_H
#define WSYSTEM_H

#include <QWidget>
#include "dashboard/Managers/info_manager.h"

namespace Ui {
class wSystem;
}

class wSystem : public QWidget
{
    Q_OBJECT

public:
    explicit wSystem(QWidget *parent = 0);
    ~wSystem();

private:
    Ui::wSystem *ui;
    InfoManager *info;

    void checkSystem();
};

#endif // WSYSTEM_H
