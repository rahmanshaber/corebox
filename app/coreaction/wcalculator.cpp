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

#include "wcalculator.h"
#include "ui_wcalculator.h"

float varA, varB, result;
int z = 0, varC, varD;

wCalculator::wCalculator(QWidget *parent) :QWidget(parent),ui(new Ui::wCalculator)
{
    ui->setupUi(this);
    ui->calcview->setValidator(new QDoubleValidator(0,99999999,99999999,this));
}

wCalculator::~wCalculator()
{
    delete ui;
}

//--------------------Calculator----------------------Start---------------------------
void wCalculator::on_one_clicked()
{
    ui->calcview->setText(ui->calcview->text() + "1");
}

void wCalculator::on_two_clicked()
{
    ui->calcview->setText(ui->calcview->text() + "2");
}

void wCalculator::on_three_clicked()
{
    ui->calcview->setText(ui->calcview->text() + "3");
}

void wCalculator::on_four_clicked()
{
    ui->calcview->setText(ui->calcview->text() + "4");
}

void wCalculator::on_five_clicked()
{
    ui->calcview->setText(ui->calcview->text() + "5");
}

void wCalculator::on_six_clicked()
{
    ui->calcview->setText(ui->calcview->text() + "6");
}

void wCalculator::on_seven_clicked()
{
    ui->calcview->setText(ui->calcview->text() + "7");
}

void wCalculator::on_eight_clicked()
{
    ui->calcview->setText(ui->calcview->text() + "8");
}

void wCalculator::on_nine_clicked()
{
    ui->calcview->setText(ui->calcview->text() + "9");
}

void wCalculator::on_zero_clicked()
{
    ui->calcview->setText(ui->calcview->text() + "0");
}

void wCalculator::on_dot_clicked()
{
    ui->calcview->setText(ui->calcview->text() + ".");
}

void wCalculator::on_equal_clicked()
{
    varB = ui->calcview->text().toFloat();
    switch(z)
    {
    case 1 : result = varA + varB ;break;
    case 2 : result = varA - varB ;break;
    case 3 : result = varA * varB ;break;
    case 4 : result = varA / varB ;break;
    case 5 :
        varC = varA;
        varD = varB;
        result = varC % varD ;break;
    }
    QString sss = QString::number(result);
    ui->calcview->setText(sss);
}

void wCalculator::on_clear_clicked()
{
    ui->calcview->setText("");
}

void wCalculator::on_multiply_clicked()
{
    z = 3;
    varA = ui->calcview->text().toFloat();
    ui->calcview->setText("");
}

void wCalculator::on_subtract_clicked()
{
    z = 2;
    varA = ui->calcview->text().toFloat();
    ui->calcview->setText("");
}

void wCalculator::on_add_clicked()
{
    z = 1;
    varA = ui->calcview->text().toFloat();
    ui->calcview->setText("");
}

void wCalculator::on_division_clicked()
{
    z = 4;
    varA = ui->calcview->text().toFloat();
    ui->calcview->setText("");
}

//void wCalculator::on_percentage_clicked()
//{
//    z = 5;
//    varA = ui->calcview->text().toFloat();
//    ui->calcview->setText("");
//}
//--------------------Calculator----------------------End---------------------------
