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

#include "coreshot.h"
#include "ui_coreshot.h"
#include "corebox/globalfunctions.h"

coreshot::coreshot(QWidget *parent) :QWidget(parent),ui(new Ui::coreshot)
{
    ui->setupUi(this);

    int x = screensize().width()  * .55;
    int y = screensize().height() * .6;
    this->resize(x, y);

    files = "";
}

coreshot::~coreshot()
{
    delete ui;
}

void coreshot::setPixmap(QPixmap &pix)
{
    ui->shotPreview->setOriginalPixmap(pix);
}

void coreshot::on_openInCorePaint_clicked()
{
    QString fileName = sm.getSCSaveLocation() + "/Screenshot_" + QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss") + ".png";
    QFile file(fileName);
    ui->shotPreview->originalPixmap().save(&file, "PNG");
    file.close();
    files = fileName;
    CoreBox *cBox = new CoreBox();
    cBox->tabEngine(CorePaint, files);
    this->close();
    cBox->show();
}

void coreshot::on_save_clicked()
{
    QString fileName = sm.getSCSaveLocation();
    QFile file(fileName + "/Screenshot_" + QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss") + ".png");
    ui->shotPreview->originalPixmap().save(&file, "PNG");
    file.close();
    files = fileName;
    // Function from globalfunctions.cpp
    messageEngine("Screenshot Saved", MessageType::Info);
    this->close();
}

void coreshot::on_saveAs_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this,tr("Save Screenshot"),sm.getSCSaveLocation(), tr("Image (*.png)"));
    QFile file(filename + ".png");

    ui->shotPreview->originalPixmap().save(&file, "PNG");

    file.close();
    files = filename;
    // Function from globalfunctions.cpp
    messageEngine("Screenshot Saved", MessageType::Info);
    this->close();
}

void coreshot::on_cancel_clicked()
{
    this->close();
}

void coreshot::on_newShot_clicked()
{
    coreshotdialog *shotD = new coreshotdialog();
    shotD->show();
    this->close();
}

void coreshot::on_openInCoreImage_clicked()
{
    QString fileName = sm.getSCSaveLocation() + "/Screenshot_" + QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss") + ".png";
    QFile file(fileName);
    ui->shotPreview->originalPixmap().save(&file, "PNG");
    file.close();
    files = fileName;
    CoreBox *cBox = new CoreBox();
    cBox->tabEngine(CoreImage, files);
    this->close();
    cBox->show();
}
