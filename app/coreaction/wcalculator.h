#ifndef WCALCULATOR_H
#define WCALCULATOR_H

#include <QWidget>
#include <QDoubleValidator>

namespace Ui {
class wCalculator;
}

class wCalculator : public QWidget
{
    Q_OBJECT

public:
    explicit wCalculator(QWidget *parent = 0);
    ~wCalculator();

private:
    Ui::wCalculator *ui;

private slots:
    //void on_percentage_clicked();

    void on_one_clicked();
    void on_two_clicked();
    void on_three_clicked();
    void on_four_clicked();
    void on_five_clicked();
    void on_six_clicked();
    void on_seven_clicked();
    void on_eight_clicked();
    void on_nine_clicked();
    void on_zero_clicked();
    void on_dot_clicked();
    void on_equal_clicked();
    void on_clear_clicked();
    void on_multiply_clicked();
    void on_subtract_clicked();
    void on_add_clicked();
    void on_division_clicked();

};

#endif // WCALCULATOR_H
