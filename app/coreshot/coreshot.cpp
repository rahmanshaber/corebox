#include "coreshot.h"
#include "ui_coreshot.h"

#include <QFileDialog>
#include "coreshot/coreshotdialog.h"

coreshot::coreshot(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::coreshot)
{
    ui->setupUi(this);

    files = "";
}

coreshot::~coreshot()
{
    delete ui;
}

void coreshot::setPixmap(QPixmap &pix) {
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
    messageEngine("Screenshot Saved", "Info");
    this->close();
}

void coreshot::on_saveAs_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this,tr("Save Screenshot"),sm.getSCSaveLocation(), tr("Image (*.png)"));
    QFile file(filename + ".png");

    ui->shotPreview->originalPixmap().save(&file, "PNG");

    file.close();
    files = filename;
    messageEngine("Screenshot Saved", "Info");
    this->close();
}

void coreshot::on_cancel_clicked()
{
    this->close();
}

void coreshot::on_newShot_clicked()
{
    CoreShotDialog *shotD = new CoreShotDialog();
    shotD->show();
    this->close();
}
