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

#ifndef COREPAINT_H
#define COREPAINT_H

#include "easypaintenums.h"

#include <QWidget>
#include <QMap>
#include <QMenu>

#include "settingsmanage.h"
#include "globalfunctions.h"

QT_BEGIN_NAMESPACE
class QAction;
class QTabWidget;
class PaletteBar;
class ImageArea;
class QLabel;
class QUndoGroup;
class ColorChooser;
QT_END_NAMESPACE


namespace Ui {
class corepaint;
}

class corepaint : public QWidget
{
    Q_OBJECT

public:
    explicit corepaint(QWidget *parent = 0);
    ~corepaint();
    void initializeNewTab(const bool &isOpen = false, const QString &filePath = "");

    QString saveLocation;

protected:
    void closeEvent(QCloseEvent *event);

private:
    SettingsManage sm;
    Ui::corepaint *ui;
    void initializeMainMenu();
    void loadSettings();
    void shotcuts();

    ImageArea* getCurrentImageArea();
    ImageArea* getImageAreaByIndex(int index);
    bool closeAllTabs();
    bool isSomethingModified();

    QMap<InstrumentsEnum, QAction*> mInstrumentsActMap;
    QUndoGroup *mUndoStackGroup;
    bool mPrevInstrumentSetted;

    ColorChooser *mPColorChooser, *mSColorChooser;
    QString filepath;
    QString currentFile;

private slots:
    void setNewSizeToSizeLabel(const QSize &size);
    void setNewPosToPosLabel(const QPoint &pos);
    void setCurrentPipetteColor(const QColor &color);
    void clearStatusBarColor();
    void setInstrumentChecked(InstrumentsEnum instrument);
    void setAllInstrumentsUnchecked(QAction *action);
    void instumentsAct(bool state);
    void enableCopyCutActions(bool enable);
    void clearImageSelection();
    void restorePreviousInstrument();
    void setInstrument(InstrumentsEnum instrument);
    void on_paintTabs_currentChanged(int index);
    void on_paintTabs_tabCloseRequested(int index);
    void on_newtab_clicked();
    void on_open_clicked();
    void on_save_clicked();
    void on_saveas_clicked();
    void on_resizeimage_clicked();
    void on_resizecanvas_clicked();
    void on_rotateright_clicked();
    void on_rotateleft_clicked();
    void on_zoomin_clicked();
    void on_zoomout_clicked();
    void penValueChanged(const int &value);
    void primaryColorChanged(const QColor &color);
    void secondaryColorChanged(const QColor &color);
    void on_bookMarkIt_clicked();
    void on_cut_clicked();
    void on_copy_clicked();
    void on_past_clicked();
    void on_openshots_clicked();
    void undo();
    void redo();

signals:
    void sendInstrumentChecked(InstrumentsEnum);
    void sendClearStatusBarColor();
    void sendClearImageSelection();

public slots:
    void setPrimaryColorView();
    void setSecondaryColorView();   

};

#endif // COREPAINT_H
