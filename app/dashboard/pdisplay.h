#ifndef PDISPLAY_H
#define PDISPLAY_H

#include <QWidget>

namespace Ui {
class pDisplay;
}

class pDisplay : public QWidget
{
    Q_OBJECT

public:
    explicit pDisplay(QWidget *parent = 0);
    ~pDisplay();

private:
    Ui::pDisplay *ui;

    void setupDisplayPage();
};

#endif // PDISPLAY_H
