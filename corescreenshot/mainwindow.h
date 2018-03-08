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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "modeselectarea.h"
#include "modefullscreen.h"
#include "previewwidget.h"

#include <QMainWindow>
#include <QtWidgets>
#include <QTimer>

#include "corepaint/corepaint.h"
#include "../corebox/corebox.h"
#include "../settings/settingsmanage.h"
#include "../corebox/globalfunctions.h"


class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    CoreBox* cBox() const {
        return cBox_;
    }

public slots:
    void waitAndTakeScreen();
    void takeScreen();
    void saveAs();
    void toCorePaint();
    void save();
    void emitToS();
    void on_cType_IndexChanged(int index);

protected :
    void addSnapMode(ScreenWidget * widget);

protected slots:
    void setPreview();

private:
    SettingsManage sm;
    void delaying(int secs);
    CoreBox * cBox_;
    PreviewWidget * cPreview;
    QPushButton * cShot;
    QPushButton * cSave;
    QPushButton * cCancel;
    QPushButton * cCorePaint;
    QPushButton * cSaveAs;
    QComboBox * cType;
    QLabel * cLable;
    QSpinBox * cdelaySec;
    QLabel * cdelaylbl;
    QString files;
    QList<ScreenWidget*> cSnapWidgets;

};

#endif // MAINWINDOW_H
