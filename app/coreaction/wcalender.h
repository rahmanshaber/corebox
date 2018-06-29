#ifndef WCALENDER_H
#define WCALENDER_H

#include <QWidget>

namespace Ui {
class wCalender;
}

class wCalender : public QWidget
{
    Q_OBJECT

public:
    explicit wCalender(QWidget *parent = 0);
    ~wCalender();

private:
    Ui::wCalender *ui;
};

#endif // WCALENDER_H
