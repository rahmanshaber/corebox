#ifndef CIRCLEBAR_H
#define CIRCLEBAR_H

#include <QWidget>
#include <QtCharts>


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
    void setValue(int value, QString valueText);

private slots:
    void init();

private:
    Ui::CircleBar *ui;
    QStringList colors;

    QChart *chart;
    QChartView *chartView;
    QPieSeries *series;
};

#endif // CIRCLEBAR_H
