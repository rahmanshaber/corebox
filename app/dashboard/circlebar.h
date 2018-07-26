#ifndef CIRCLEBAR_H
#define CIRCLEBAR_H

#include "dashboard/signal_mapper.h"

#include <QWidget>
#include <QtCharts>

#include "corebox/globalfunctions.h"


namespace Ui {
    class CircleBar;
}

class CircleBar : public QWidget
{
    Q_OBJECT

public:
    explicit CircleBar(const QString &title, const QStringList &colors, QWidget *parent = 0);
    ~CircleBar();

public slots:
    void setValue(const int &value, const QString &valueText);

private slots:
    void init();

private:
    Ui::CircleBar *ui;

    QStringList mColors;
    QChart *mChart;
    QChartView *mChartView;
    QPieSeries *mSeries;
};

#endif // CIRCLEBAR_H
