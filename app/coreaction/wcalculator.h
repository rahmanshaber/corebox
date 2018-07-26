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

#ifndef WCALCULATOR_H
#define WCALCULATOR_H

#include <QWidget>
#include <QDebug>
#include <QDoubleValidator>

#include "corebox/globalfunctions.h"


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
