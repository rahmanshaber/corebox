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

#ifndef RISEZEDIALOG_H
#define RISEZEDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QGridLayout>
#include <QRadioButton>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <QCheckBox>
#include <QGroupBox>
#include <QVBoxLayout>

QT_BEGIN_NAMESPACE
class QLabel;
class QSpinBox;
class QCheckBox;
QT_END_NAMESPACE


class ResizeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ResizeDialog(const QSize &size, QWidget *parent);
    inline QSize getNewSize() { return QSize(mWidth, mHeight); }
    
private:
    void initializeGui();

    QLabel *mNewSizeLabel; /**< Label for showing new size */
    QSpinBox *mPixelWButton, *mPixelHButton,*mPercentWButton, *mPercentHButton;
    QCheckBox *mPreserveAspectBox;
    int mWidth, mHeight,mOrigWidth, mOrigHeight;

private slots:
    void pixelsButtonClicked(bool flag);
    void percentButtonClicked(bool flag);
    void pixelsWValueChanged(const int &value);
    void pixelsHValueChanged(const int &value);
    void percentWValueChanged(const int &value);
    void percentHValueChanged(const int &value);
    
};

#endif // RISEZEDIALOG_H
