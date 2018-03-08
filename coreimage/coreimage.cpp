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

#include "coreimage.h"
#include "../corebox/corebox.h"
#include "ui_coreimage.h"

#include <QFileDialog>
#include <QStandardPaths>
#include <QImageReader>
#include <QImageWriter>
#include <QTransform>
#include <QFileInfo>
#include <QBuffer>
#include <QDataStream>
#include <QShortcut>
#include <QAbstractScrollArea>
#include <QSpacerItem>
#include <QtWidgets>
#include <QDateTime>


coreimage::coreimage(QWidget *parent) :QWidget(parent),ui(new Ui::coreimage)
  ,slideShowTimer(nullptr)
{
    qDebug() << "coreimage opening";
    ui->setupUi(this);

    scaleFactor = 1.0;

    ui->cImageLabel->setBackgroundRole(QPalette::Base);
    ui->cImageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    ui->scrollArea->setBackgroundRole(QPalette::Dark);
    ui->scrollArea->setStyleSheet("background-color:rgb(0,0,0);");

    ui->tools->setVisible(false);
    ui->propbox->setVisible(false);
    ui->thumnailView->setVisible(false);

    if(currentImagePath.isNull()){
        for (QPushButton *b : ui->tools->findChildren<QPushButton*>()){
            b->setEnabled(false);
        }

        for (QPushButton *b : ui->navigation->findChildren<QPushButton*>()){
            b->setEnabled(false);
        }
        ui->cTools->setEnabled(true);
        ui->cOpen->setEnabled(true);
    }
    shotcuts();
}

coreimage::~coreimage()
{
    qDebug()<<"coreimage closing";
    if(slideShowTimer)
      delete slideShowTimer;
    delete ui;
}

void coreimage::shotcuts()
{
    QShortcut* shortcut;
    shortcut = new QShortcut(QKeySequence(Qt::Key_Left), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(on_cPrevious_clicked()));
    shortcut = new QShortcut(QKeySequence(Qt::Key_Right), this);
    connect(shortcut, &QShortcut::activated, this, &coreimage::on_cNext_clicked);
    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_O), this);
    connect(shortcut, &QShortcut::activated, this, &coreimage::on_cOpen_clicked);
    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_S), this);
    connect(shortcut, &QShortcut::activated, this, &coreimage::on_cSave_clicked);
    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_S), this);
    connect(shortcut, &QShortcut::activated, this, &coreimage::on_cSaveAs_clicked);
    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_T), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(on_openThumbview_clicked()));
    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_D), this);
    connect(shortcut, &QShortcut::activated, this, &coreimage::on_cTrashIt_clicked);
    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Plus), this);
    connect(shortcut, &QShortcut::activated, this, &coreimage::on_cZoomIn_clicked);
    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Minus), this);
    connect(shortcut, &QShortcut::activated, this, &coreimage::on_cZoomOut_clicked);

//    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Space), this);
//    connect(shortcut, &QShortcut::activated, this, &coreimage::on_slideShow_clicked);
    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_B), this);
    connect(shortcut, &QShortcut::activated, this, &coreimage::on_bookMarkIt_clicked);
}

void coreimage::closeEvent(QCloseEvent *event) {
    event->ignore();
    saveToRecent("CoreImage", currentImagePath);
    event->accept();
}

void coreimage::wheelEvent(QWheelEvent *event) {
    int delta = event->delta();
    ui->scrollArea->verticalScrollBar()->setEnabled(false);
    ui->scrollArea->horizontalScrollBar()->setEnabled(false);
    if(Qt::RightButton & event->buttons()) {
      if(delta > 0) { // forward
        on_cZoomIn_clicked();
      }
      else if(delta < 0) { // backward
        on_cZoomOut_clicked();
      }
    } else {
        on_cNormalSize_clicked();
        if (delta < 0) {
            on_cNext_clicked();
        } else if (delta > 0) {
            on_cPrevious_clicked();
        }
    }
}

QStringList coreimage::getImages(QString path) {
    QDir dir(path);
    //QStringList images;
    for (QString file : dir.entryList()) {
        if (file == "." || file == "..") {
            continue;
        }

        const QByteArrayList supportedMime = QImageReader::supportedImageFormats();
        foreach (const QByteArray &mimeTypeName, supportedMime) {
            if (QFileInfo(file).completeSuffix() == mimeTypeName) {
                images.append(path + "/" + file);
            }
        }
    }
    return images;
}

bool coreimage::loadFile(const QString &fileName)
{
    if (!fileName.isNull()) {
        currentImagePath = fileName;
        QImageReader reader(fileName);
        reader.setAutoTransform(true);
        const QImage newImage = reader.read();
        if (newImage.isNull()) {
            messageEngine(tr("Cannot load %1: %2")
                          .arg(QDir::toNativeSeparators(fileName), reader.errorString()),"Warning");
            return false;
        }
        setImage(newImage);
        setWindowFilePath(fileName);

        for (QPushButton *b : ui->tools->findChildren<QPushButton*>()){
            b->setEnabled(true);
        }

        for (QPushButton *b : ui->navigation->findChildren<QPushButton*>()){
            b->setEnabled(true);
        }

        return true;
    }
    return false;
}

void coreimage::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
    scrollBar->setValue(int(factor * scrollBar->value()
                            + ((factor - 1) * scrollBar->pageStep()/2)));
}

void coreimage::setImage(const QImage &newImage)
{
    image = newImage;
    scaleFactor = 1.0;

    ui->scrollArea->setVisible(true);
    ui->navigation->setVisible(true);
    ui->horizontalLayout_3->removeItem(hSpacer);
    ui->scrollArea->setWidget(ui->cImageLabel);

    int iw = image.width();
    int ih = image.height();
    int cw = ui->scrollArea->width();
    int ch = ui->scrollArea->height();

    if (image.height() < ui->scrollArea->height() && image.width() < ui->scrollArea->width()){
        ui->cImageLabel->setPixmap((QPixmap::fromImage(image)));
        ui->cImageLabel->adjustSize();
    }
    else{
        ui->cImageLabel->setPixmap(QPixmap::fromImage(image));
        QSize t2(iw, ih);
        t2.scale(cw, ch, Qt::KeepAspectRatio);
        ui->cImageLabel->resize(t2);
    }
    if (!(images.count() > 0)) {
        QStringList l = getImages(QFileInfo(currentImagePath).path());
        for (int i = 0; i < l.count(); ++i) {
            QListWidgetItem *item = new QListWidgetItem(QIcon(QPixmap(l.at(i))), l.at(i));
            item->setFont(QFont(item->font().family(), 1));
            ui->thumnailView->addItem(item);
        }
    }

    QFileInfo info (currentImagePath);
    QString typ = info.suffix();
    QString nam = info.fileName();
    QString h = QString::number(image.height()) ;
    QString w = QString::number(image.width()) ;

    ui->name->setText("Name : " + nam + " ; ");
    ui->height->setText("Height : " + h + " px ; ");
    ui->width->setText("Width : " + w + " px ; ");
    ui->size->setText("Size : " + formatSize(info.size()) + " ; ");
    ui->type->setText("Type : " + typ + " ; ");
    ui->thumnailView->item(images.indexOf(currentImagePath))->setSelected(true);
}

void coreimage::resizeEvent(QResizeEvent *event) {

    Q_UNUSED(event);
    int iw = image.width();
    int ih = image.height();
    int cw = ui->scrollArea->width();
    int ch = ui->scrollArea->height();

    if (image.height() < ui->scrollArea->height() && image.width() < ui->scrollArea->width()){
        ui->cImageLabel->setPixmap((QPixmap::fromImage(image)));
        ui->cImageLabel->adjustSize();
    }
    else{
        ui->cImageLabel->setPixmap(QPixmap::fromImage(image));
        QSize t2(iw, ih);
        t2.scale(cw, ch, Qt::KeepAspectRatio);
        ui->cImageLabel->resize(t2);
    }
}

static void initializeImageFileDialog(QFileDialog &dialog, QFileDialog::AcceptMode acceptMode)
{
    static bool firstDialog = true;

    if (firstDialog) {
        firstDialog = false;
        const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
        dialog.setDirectory(picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.last());
    }

    dialog.setAcceptMode(acceptMode);
    QStringList mimeTypeFilters;
    const QByteArrayList supportedMimeTypes = acceptMode == QFileDialog::AcceptOpen
        ? QImageReader::supportedMimeTypes() : QImageWriter::supportedMimeTypes();
    foreach (const QByteArray &mimeTypeName, supportedMimeTypes)
        mimeTypeFilters.append(mimeTypeName);
    mimeTypeFilters.sort();
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.selectMimeTypeFilter("image/png");
    if (acceptMode == QFileDialog::AcceptSave)
       { dialog.setDefaultSuffix(dialog.selectedNameFilter());
    }
}

void coreimage::scaleImage(double factor)
{
    Q_ASSERT(ui->cImageLabel->pixmap());
    scaleFactor *= factor;
    ui->cImageLabel->resize(scaleFactor * ui->cImageLabel->pixmap()->size());

    adjustScrollBar(ui->scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(ui->scrollArea->verticalScrollBar(), factor);
}

void coreimage::on_cOpen_clicked()
{
    QFileDialog dialog(this, tr("Open File"));
    initializeImageFileDialog(dialog, QFileDialog::AcceptOpen);
    while (dialog.exec() == QDialog::Accepted && !loadFile(dialog.selectedFiles().first())) {}
}

void coreimage::on_cZoomIn_clicked()
{
    scaleImage(1.05);
}

void coreimage::on_cZoomOut_clicked()
{
    scaleImage(0.95);
}

void coreimage::on_cNormalSize_clicked()
{
    ui->cImageLabel->adjustSize();
    scaleFactor = 1.0;
}

void coreimage::on_cRotateLeft_clicked()
{
    QTransform tran;
    tran.rotate(-90);
    image = image.transformed(tran);
    ui->cImageLabel->setPixmap(QPixmap::fromImage(image));
}

void coreimage::on_cRotateRight_clicked()
{
    QTransform tran;
    tran.rotate(90);
    image = image.transformed(tran);
    ui->cImageLabel->setPixmap(QPixmap::fromImage(image));
}

void coreimage::on_cFlipVertical_clicked()
{
    QTransform tran;
    tran.scale(1, -1);
    tran.translate(0, -image.rect().height());
    image = image.transformed(tran);
    ui->cImageLabel->setPixmap(QPixmap::fromImage(image));
}

void coreimage::on_cFlipHorizontal_clicked()
{
    QTransform tran;
    tran.scale(-1, 1);
    tran.translate(-image.rect().width(), 0);
    image = image.transformed(tran);
    ui->cImageLabel->setPixmap(QPixmap::fromImage(image));
}

bool coreimage::saveFile(const QString &fileName)
{
    QImageWriter writer(fileName);

    if (!writer.write(image)) {
        messageEngine(tr("Cannot write %1: %2").arg(QDir::toNativeSeparators(fileName)).arg(writer.errorString()), "Info");

        return false;
    } else {
        messageEngine("Image Saved", "Info");
    }
    return true;
}

void coreimage::on_cSave_clicked()
{
    QImageWriter wr(currentImagePath);
    if (wr.write(image)) {
        messageEngine("Image Saved", "Info");
    }
}

void coreimage::on_cSaveAs_clicked()
{
    QFileDialog dialog(this, tr("Save File As"));
    initializeImageFileDialog(dialog, QFileDialog::AcceptSave);
    while (dialog.exec() == QDialog::Accepted && !saveFile(dialog.selectedFiles().first())) {}
}

void coreimage::on_cTools_clicked(bool checked)
{
    if(checked){
        ui->tools->setVisible(true);
    }
    else{
        ui->tools->setVisible(false);
    }
}

void coreimage::on_bookMarkIt_clicked()
{
    if (!currentImagePath.isNull()) {
        QString myIcon = ":/icons/CoreImage.svg";
        bookmarks bookMarks;
        bookMarks.callBookMarkDialog(this, currentImagePath, myIcon);
    }
}

void coreimage::on_cPrevious_clicked()
{
    //QStringList images = getImages(QFileInfo(currentImagePath).path());
    if (!images.count() == 0) {
        int currentIndex = images.indexOf(currentImagePath);
        if (!currentIndex == 0) {
            loadFile(images.at(currentIndex - 1));
        } else {
            loadFile(images.at(images.count() - 1));
        }
    }
}

void coreimage::on_cNext_clicked()
{
    //QStringList images = getImages(QFileInfo(currentImagePath).path());
    if (!images.count() == 0) {
        int currentIndex = images.indexOf(currentImagePath);
        int lastIndex = images.count() - 1;
        if (!(currentIndex == lastIndex)) {
            loadFile(images.at(currentIndex + 1));
        } else {
            loadFile(images.at(0));
        }
    }
}

void coreimage::on_slideShow_clicked(bool checked)
{
    if(checked) {
      on_cNormalSize_clicked();
      if(!slideShowTimer) {
        slideShowTimer = new QTimer();
        // switch to the next image when timeout
        connect(slideShowTimer, &QTimer::timeout, this, &coreimage::on_cNext_clicked);
      }
      ui->scrollArea->showFullScreen();
      slideShowTimer->start(3000);
      ui->tools->setVisible(false);
    }
    else {
      if(slideShowTimer) {
        delete slideShowTimer;
        slideShowTimer = nullptr;
      }
    }
}

void coreimage::on_cProperties_clicked(bool checked)
{
    if(checked){
        ui->propbox->setVisible(true);
    }
    else{
        ui->propbox->setVisible(false);
    }
}

void coreimage::on_openincorepaint_clicked()
{
    QTabWidget *boxTab = this->parent()->parent()->parent()->parent()->findChild<QTabWidget*>("windows");
    corepaint *ima = new corepaint;
    QString path = currentImagePath;

    ima->initializeNewTab(true,path);
    boxTab->insertTab(boxTab->count(), ima, QIcon(":/icons/CorePaint.svg"), "CorePaint");
    boxTab->setCurrentIndex(boxTab->count());
}

void coreimage::on_openThumbview_clicked()
{
    if (ui->openThumbview->isChecked()) {
        ui->thumnailView->setVisible(true);
    }
    else {
        ui->thumnailView->setVisible(false);
    }
}

void coreimage::on_containingfolder_clicked()
{
    CoreBox *cBox = qobject_cast<CoreBox*>(qApp->activeWindow());
    cBox->tabEngine(0, QFileInfo(currentImagePath).path());
}

void coreimage::on_thumnailView_itemClicked(QListWidgetItem *item)
{
    loadFile(images.at(ui->thumnailView->row(item)));
}

void coreimage::mousePressEvent(QMouseEvent *event)
{
  mousePressed = true;
  mousePos = event->globalPos();

  if (ui->cImageLabel->underMouse()){
      wndPos = ui->cImageLabel->pos();
  }
}

void coreimage::mouseMoveEvent(QMouseEvent *event)
{
  if (ui->cImageLabel->underMouse() && mousePressed){
      ui->cImageLabel->move(wndPos + (event->globalPos() - mousePos));
  }
}

void coreimage::mouseReleaseEvent(QMouseEvent *event)
{
  Q_UNUSED(event);
  mousePressed = false;
}

void coreimage::on_cTrashIt_clicked()
{
    moveToTrash(currentImagePath);
}
