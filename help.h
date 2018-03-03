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

#ifndef HELP_H
#define HELP_H

#include <QWidget>
#include <QPushButton>


namespace Ui {
class help;
}

class help : public QWidget
{
    Q_OBJECT

public:
    explicit help(QWidget *parent = 0);
    ~help();

private slots:
    void pageClick(QPushButton *btn, int i, QString title);
    void on_helpus_clicked();
    void on_search_clicked();
    void on_start_clicked();
    void on_dashboard_clicked();
    void on_coreplayer_clicked();
    void on_corerenamer_clicked();
    void on_coretime_clicked();
    void on_corescrshot_clicked();
    void on_corepaint_clicked();
    void on_coreimage_clicked();
    void on_corepad_clicked();
    void on_corefm_clicked();
    void on_corebox_clicked();

private:
    Ui::help *ui;
};

#endif // HELP_H
