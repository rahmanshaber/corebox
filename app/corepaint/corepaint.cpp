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

#include "corepaint.h"
#include "ui_corepaint.h"
#include "imagearea.h"
#include "datasingleton.h"
#include "widgets/colorchooser.h"

#include <QApplication>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QScrollArea>
#include <QLabel>
#include <QtEvents>
#include <QPainter>
#include <QInputDialog>
#include <QUndoGroup>
#include <QtCore/QTimer>
#include <QtCore/QMap>
#include <QDateTime>
#include <QSettings>


corepaint::corepaint( QWidget *parent):QWidget(parent),
    ui(new Ui::corepaint), mPrevInstrumentSetted(false)
{
    ui->setupUi(this);

    mUndoStackGroup = new QUndoGroup(this);

    qRegisterMetaType<InstrumentsEnum>("InstrumentsEnum");
    DataSingleton::Instance()->setIsInitialized();

    if(filepath.isEmpty()){
        ui->page->setTabEnabled(1,false);
        ui->page->setTabEnabled(2,false);
        ui->save->setEnabled(false);
        ui->saveas->setEnabled(false);
        ui->bookMarkIt->setEnabled(false);
    }

    initializeMainMenu();
    shotcuts();
    loadSettings();
}

corepaint::~corepaint()
{
    delete ui;
}

void corepaint::loadSettings() {
    saveLocation = sm.getSCSaveLocation();
}

void corepaint::shotcuts()
{
    QShortcut* shortcut;
    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_O), this);
    connect(shortcut, &QShortcut::activated, this, &corepaint::on_open_clicked);
    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_S), this);
    connect(shortcut, &QShortcut::activated, this, &corepaint::on_save_clicked);
    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_S), this);
    connect(shortcut, &QShortcut::activated, this, &corepaint::on_saveas_clicked);

    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Z), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(undo()));
    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Y), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(redo()));

    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_C), this);
    connect(shortcut, &QShortcut::activated, this, &corepaint::on_copy_clicked);
    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_X), this);
    connect(shortcut, &QShortcut::activated, this, &corepaint::on_cut_clicked);
    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_V), this);
    connect(shortcut, &QShortcut::activated, this, &corepaint::on_past_clicked);
    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_N), this);
    connect(shortcut, &QShortcut::activated, this, &corepaint::on_newtab_clicked);
    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_B), this);
    connect(shortcut, &QShortcut::activated, this, &corepaint::on_bookMarkIt_clicked);

    shortcut = new QShortcut(QKeySequence(Qt::Key_Delete), this);
    connect(shortcut, &QShortcut::activated, this, &corepaint::on_delet_clicked);
}

void corepaint::initializeNewTab(const bool &isOpen, const QString &filePath)
{
    if (ui->paintTabs->count() < 10) {
        ImageArea *imageArea;
        QString fileName;

        if(isOpen && filePath.isEmpty())
        {
            imageArea = new ImageArea(isOpen, "", this);
            fileName = imageArea->getFileName();
        } else if(isOpen && !filePath.isEmpty()) {
            imageArea = new ImageArea(isOpen, filePath, this);
            fileName = imageArea->getFileName();
        } else {
            imageArea = new ImageArea(false, "", this);
            fileName = (tr("UntitledImage_") + QString::number(ui->paintTabs->count()));
        }
        if (!imageArea->getFileName().isNull()) {
            QScrollArea *scrollArea = new QScrollArea();
            scrollArea->setAttribute(Qt::WA_DeleteOnClose);
            scrollArea->setBackgroundRole(QPalette::Dark);
            scrollArea->setWidget(imageArea);

            ui->paintTabs->addTab(scrollArea,fileName);
            ui->paintTabs->setCurrentIndex(ui->paintTabs->count()-1);

            mUndoStackGroup->addStack(imageArea->getUndoStack());
            connect(imageArea, SIGNAL(sendPrimaryColorView()), this, SLOT(setPrimaryColorView()));
            connect(imageArea, SIGNAL(sendSecondaryColorView()), this, SLOT(setSecondaryColorView()));
            connect(imageArea, SIGNAL(sendRestorePreviousInstrument()), this, SLOT(restorePreviousInstrument()));
            connect(imageArea, SIGNAL(sendSetInstrument(InstrumentsEnum)), this, SLOT(setInstrument(InstrumentsEnum)));
            connect(imageArea, SIGNAL(sendNewImageSize(QSize)), this, SLOT(setNewSizeToSizeLabel(QSize)));
            connect(imageArea, SIGNAL(sendCursorPos(QPoint)), this, SLOT(setNewPosToPosLabel(QPoint)));
            connect(imageArea, SIGNAL(sendColor(QColor)), this, SLOT(setCurrentPipetteColor(QColor)));
            connect(imageArea, SIGNAL(sendEnableCopyCutActions(bool)), this, SLOT(enableCopyCutActions(bool)));
            connect(imageArea, SIGNAL(sendEnableSelectionInstrument(bool)), this, SLOT(instumentsAct(bool)));

            //setWindowTitle(QString("%1 - EasyPaint").arg(fileName));
        } else {
            delete imageArea;
        }

        currentFile = fileName;
        filepath = filePath;
        if (ui->paintTabs->count() >= 1) {
            ui->page->setTabEnabled(1, true);
            ui->page->setTabEnabled(2,true);
            ui->save->setEnabled(true);
            ui->saveas->setEnabled(true);
            ui->bookMarkIt->setEnabled(true);
        }
        if (!fileName.isEmpty()) {
            messageEngine("File Opened Successfully.", "Info");
        } else {
            messageEngine("File not Opened Successfully.", "Info");
        }
    }
    else {
        messageEngine("Reached page limite.", "Warning");
    }
}

void corepaint::initializeMainMenu()
{
    ui->actioniundo = mUndoStackGroup->createUndoAction(this);
    ui->undo->setDefaultAction(ui->actioniundo);
    ui->actioniredo = mUndoStackGroup->createRedoAction(this);
    ui->redo->setDefaultAction( ui->actioniredo);

    connect(ui->actionCursor, SIGNAL(triggered(bool)), this, SLOT(instumentsAct(bool)));
    mInstrumentsActMap.insert(CURSOR, ui->actionCursor);
    ui->cursor->setDefaultAction(ui->actionCursor);

    connect(ui->actionEraser, SIGNAL(triggered(bool)), this, SLOT(instumentsAct(bool)));
    mInstrumentsActMap.insert(ERASER, ui->actionEraser);
    ui->eraser->setDefaultAction(ui->actionEraser);

    connect(ui->actionColorpicker, SIGNAL(triggered(bool)), this, SLOT(instumentsAct(bool)));
    mInstrumentsActMap.insert(COLORPICKER, ui->actionColorpicker);
    ui->colorpicker->setDefaultAction(ui->actionColorpicker);

    connect(ui->actionMagnifier, SIGNAL(triggered(bool)), this, SLOT(instumentsAct(bool)));
    mInstrumentsActMap.insert(MAGNIFIER, ui->actionMagnifier);
    ui->magnifier->setDefaultAction(ui->actionMagnifier);

    connect(ui->actionPen, SIGNAL(triggered(bool)), this, SLOT(instumentsAct(bool)));
    mInstrumentsActMap.insert(PEN, ui->actionPen);
    ui->pen->setDefaultAction(ui->actionPen);

    connect(ui->actionLine, SIGNAL(triggered(bool)), this, SLOT(instumentsAct(bool)));
    mInstrumentsActMap.insert(LINE, ui->actionLine);
    ui->line->setDefaultAction(ui->actionLine);

    connect(ui->actionSpray, SIGNAL(triggered(bool)), this, SLOT(instumentsAct(bool)));
    mInstrumentsActMap.insert(SPRAY, ui->actionSpray);
    ui->spray->setDefaultAction(ui->actionSpray);

    connect(ui->actionFill, SIGNAL(triggered(bool)), this, SLOT(instumentsAct(bool)));
    mInstrumentsActMap.insert(FILL, ui->actionFill);
    ui->fill->setDefaultAction(ui->actionFill);

    connect(ui->actionRectangle, SIGNAL(triggered(bool)), this, SLOT(instumentsAct(bool)));
    mInstrumentsActMap.insert(RECTANGLE, ui->actionRectangle);
    ui->rectangle->setDefaultAction(ui->actionRectangle);

    connect(ui->actionEllipse, SIGNAL(triggered(bool)), this, SLOT(instumentsAct(bool)));
    mInstrumentsActMap.insert(ELLIPSE, ui->actionEllipse);
    ui->ellipse->setDefaultAction(ui->actionEllipse);

    connect(ui->actionCurve, SIGNAL(triggered(bool)), this, SLOT(instumentsAct(bool)));
    mInstrumentsActMap.insert(CURVELINE, ui->actionCurve);
    ui->curve->setDefaultAction(ui->actionCurve);

    connect(ui->actionText, SIGNAL(triggered(bool)), this, SLOT(instumentsAct(bool)));
    mInstrumentsActMap.insert(TEXT, ui->actionText);
    ui->text->setDefaultAction(ui->actionText);


    mPColorChooser = new ColorChooser(0, 0, 0, this);
    mPColorChooser->setStatusTip(tr("Primary color"));
    mPColorChooser->setToolTip(tr("Primary color"));
    connect(mPColorChooser, SIGNAL(sendColor(QColor)), this, SLOT(primaryColorChanged(QColor)));

    mSColorChooser = new ColorChooser(255, 255, 255, this);
    mSColorChooser->setStatusTip(tr("Secondary color"));
    mSColorChooser->setToolTip(tr("Secondary color"));
    connect(mSColorChooser, SIGNAL(sendColor(QColor)), this, SLOT(secondaryColorChanged(QColor)));

    ui->cc->addWidget(mPColorChooser);
    ui->cc->addWidget(mSColorChooser);

    connect(ui->pensize, SIGNAL(valueChanged(int)), this, SLOT(penValueChanged(int)));
}

void corepaint::penValueChanged(const int &value)
{
    DataSingleton::Instance()->setPenSize(value);
}

void corepaint::primaryColorChanged(const QColor &color)
{
    DataSingleton::Instance()->setPrimaryColor(color);
}

void corepaint::secondaryColorChanged(const QColor &color)
{
    DataSingleton::Instance()->setSecondaryColor(color);
}

void corepaint::setPrimaryColorView()
{
    mPColorChooser->setColor(DataSingleton::Instance()->getPrimaryColor());
}

void corepaint::setSecondaryColorView()
{
    mSColorChooser->setColor(DataSingleton::Instance()->getSecondaryColor());
}

ImageArea* corepaint::getCurrentImageArea()
{
    if (ui->paintTabs->currentWidget()){
        QScrollArea *tempScrollArea = qobject_cast<QScrollArea*>(ui->paintTabs->currentWidget());
        ImageArea *tempArea = qobject_cast<ImageArea*>(tempScrollArea->widget());
        return tempArea;
    }
    return NULL;
}

ImageArea* corepaint::getImageAreaByIndex(int index)
{
    QScrollArea *sa = static_cast<QScrollArea*>(ui->paintTabs->widget(index));
    ImageArea *ia = static_cast<ImageArea*>(sa->widget());
    return ia;
}

void corepaint::setNewSizeToSizeLabel(const QSize &size)
{
    ui->mSizeLabel->setText(QString("%1 x %2").arg(size.width()).arg(size.height()));
}

void corepaint::setNewPosToPosLabel(const QPoint &pos)
{
    ui->mPosLabel->setText(QString("%1,%2").arg(pos.x()).arg(pos.y()));
}

void corepaint::setCurrentPipetteColor(const QColor &color)
{
    ui->pColorRGBLabel->setText(QString("RGB: %1,%2,%3").arg(color.red())
                         .arg(color.green()).arg(color.blue()));

    QPixmap statusColorPixmap = QPixmap(18, 18);
    QPainter statusColorPainter;
    statusColorPainter.begin(&statusColorPixmap);
    statusColorPainter.fillRect(0, 0, 19, 19, color);
    statusColorPainter.end();
    ui->pColorPreviewLabel->setPixmap(statusColorPixmap);
}

void corepaint::clearStatusBarColor()
{
    ui->pColorPreviewLabel->clear();
    ui->pColorRGBLabel->clear();
}

void corepaint::closeEvent(QCloseEvent *event)
{
    if(!isSomethingModified() || closeAllTabs())
    {
        event->ignore();
        saveToRecent("CorePaint", currentFile);
        event->accept();
    }
    else
        event->ignore();
}

bool corepaint::isSomethingModified()
{
    for(int i = 0;i < ui->paintTabs->count(); ++i){
        if(getImageAreaByIndex(i)->getEdited())
            return true;
    }
    return false;
}

bool corepaint::closeAllTabs()
{
    while(ui->paintTabs->count() != 0)
    {
        ImageArea *ia = getImageAreaByIndex(0);
        if(ia->getEdited())
        {
            int ans = QMessageBox::warning(this, tr("Closing Tab..."),
                                           tr("File has been modified.\nDo you want to save changes?"),
                                           QMessageBox::Yes | QMessageBox::Default,
                                           QMessageBox::No, QMessageBox::Cancel | QMessageBox::Escape);
            switch(ans)
            {
            case QMessageBox::Yes:
                if (ia->save())
                    break;
                return false;
            case QMessageBox::Cancel:
                return false;
            }
        }
        QWidget *wid = ui->paintTabs->widget(0);
        ui->paintTabs->removeTab(0);
        delete wid;
    }
    return true;
}

void corepaint::setAllInstrumentsUnchecked(QAction *action)
{
    clearImageSelection();
    foreach (QAction *temp, mInstrumentsActMap)
    {
        if(temp != action)
            temp->setChecked(false);
    }
}

void corepaint::setInstrumentChecked(InstrumentsEnum instrument)
{
    setAllInstrumentsUnchecked(NULL);
    if(instrument == NONE_INSTRUMENT || instrument == INSTRUMENTS_COUNT)
        return;
    mInstrumentsActMap[instrument]->setChecked(true);
}

void corepaint::instumentsAct(bool state)
{
    QAction *currentAction = static_cast<QAction*>(sender());
    if(state)
    {
        if(currentAction == mInstrumentsActMap[COLORPICKER] && !mPrevInstrumentSetted)
        {
            DataSingleton::Instance()->setPreviousInstrument(DataSingleton::Instance()->getInstrument());
            mPrevInstrumentSetted = true;
        }
        setAllInstrumentsUnchecked(currentAction);
        currentAction->setChecked(true);
        DataSingleton::Instance()->setInstrument(mInstrumentsActMap.key(currentAction));
        emit sendInstrumentChecked(mInstrumentsActMap.key(currentAction));
    }
    else
    {
        setAllInstrumentsUnchecked(NULL);
        DataSingleton::Instance()->setInstrument(NONE_INSTRUMENT);
        emit sendInstrumentChecked(NONE_INSTRUMENT);
        if(currentAction == mInstrumentsActMap[CURSOR])
            DataSingleton::Instance()->setPreviousInstrument(mInstrumentsActMap.key(currentAction));
    }
}

void corepaint::enableCopyCutActions(bool enable)
{
    ui->copy->setEnabled(enable);
    ui->cut->setEnabled(enable);
}

void corepaint::clearImageSelection()
{
    if (getCurrentImageArea())
    {
        getCurrentImageArea()->clearSelection();
        DataSingleton::Instance()->setPreviousInstrument(NONE_INSTRUMENT);
    }
}

void corepaint::restorePreviousInstrument()
{
    setInstrumentChecked(DataSingleton::Instance()->getPreviousInstrument());
    DataSingleton::Instance()->setInstrument(DataSingleton::Instance()->getPreviousInstrument());
    emit sendInstrumentChecked(DataSingleton::Instance()->getPreviousInstrument());
    mPrevInstrumentSetted = false;
}

void corepaint::setInstrument(InstrumentsEnum instrument)
{
    setInstrumentChecked(instrument);
    DataSingleton::Instance()->setInstrument(instrument);
    emit sendInstrumentChecked(instrument);
    mPrevInstrumentSetted = false;
}

void corepaint::on_paintTabs_currentChanged(int index)
{
    //activateTab
    if(index == -1)
        return;
    ui->paintTabs->setCurrentIndex(index);
    getCurrentImageArea()->clearSelection();

    QSize size = getCurrentImageArea()->getImage()->size();
    ui->mSizeLabel->setText(QString("%1 x %2").arg(size.width()).arg(size.height()));

    if(!getCurrentImageArea()->getFileName().isEmpty())
    {
        currentFile = QDir(currentFile).path() + "/" + getCurrentImageArea()->getFileName();
    }
    else
    {
        currentFile = "Untitled Image";
    }
    mUndoStackGroup->setActiveStack(getCurrentImageArea()->getUndoStack());

    //enableActions
    //if index == -1 it means, that there is no tabs
    bool isEnable = index == -1 ? false : true;

    if(!isEnable)
    {
        setAllInstrumentsUnchecked(NULL);
        DataSingleton::Instance()->setInstrument(NONE_INSTRUMENT);
        emit sendInstrumentChecked(NONE_INSTRUMENT);
    }

    ui->workingOn->setText(ui->paintTabs->tabText(index));

}

void corepaint::on_paintTabs_tabCloseRequested(int index)
{
    ImageArea *ia = getImageAreaByIndex(index);
    if(ia->getEdited())
    {
        int ans = QMessageBox::warning(this, tr("Closing Tab..."),
                                       tr("File has been modified.\nDo you want to save changes?"),
                                       QMessageBox::Yes | QMessageBox::Default,
                                       QMessageBox::No, QMessageBox::Cancel | QMessageBox::Escape);
        switch(ans)
        {
        case QMessageBox::Yes:
            if (ia->save()) break;
            return;
        case QMessageBox::Cancel:
            return;
        }
    }
    mUndoStackGroup->removeStack(ia->getUndoStack()); //for safety
    QWidget *wid = ui->paintTabs->widget(index);
    ui->paintTabs->removeTab(index);
    delete wid;
    if(ui->paintTabs->count() == 0){
        ui->page->setTabEnabled(1,false);
        ui->page->setTabEnabled(2,false);
        ui->save->setEnabled(false);
        ui->saveas->setEnabled(false);
        ui->bookMarkIt->setEnabled(false);
        ui->selectedsection->setText("");
    }
}

void corepaint::on_newtab_clicked()
{
    initializeNewTab();
}

void corepaint::on_open_clicked()
{
    initializeNewTab(true);
}

void corepaint::on_save_clicked()
{
    if(getCurrentImageArea()->save()){
        ui->paintTabs->setTabText(ui->paintTabs->currentIndex(), getCurrentImageArea()->getFileName().isEmpty() ?
                                      tr("Untitled Image") : getCurrentImageArea()->getFileName() );

        filepath = getCurrentImageArea()->mFilePath;
        messageEngine("File Saved", "Info");
    } else {
        messageEngine("File not Saved", "Info");
    }
}

void corepaint::on_saveas_clicked()
{
    if(getCurrentImageArea()->saveAs()){
        ui->paintTabs->setTabText(ui->paintTabs->currentIndex(), getCurrentImageArea()->getFileName().isEmpty() ?
                                      tr("Untitled Image") : getCurrentImageArea()->getFileName() );

        filepath = getCurrentImageArea()->mFilePath;
        messageEngine("File Saved", "Info");
    } else {
        messageEngine("File not Saved", "Info");
    }
}

void corepaint::on_resizeimage_clicked()
{
    getCurrentImageArea()->resizeImage();
}

void corepaint::on_resizecanvas_clicked()
{
    getCurrentImageArea()->resizeCanvas();
}

void corepaint::on_rotateright_clicked()
{
    getCurrentImageArea()->rotateImage(true);
}

void corepaint::on_rotateleft_clicked()
{
    getCurrentImageArea()->rotateImage(false);
}

void corepaint::on_zoomin_clicked()
{
    getCurrentImageArea()->zoomImage(2.0);
    getCurrentImageArea()->setZoomFactor(2.0);
}

void corepaint::on_zoomout_clicked()
{
    getCurrentImageArea()->zoomImage(0.5);
    getCurrentImageArea()->setZoomFactor(0.5);
}

void corepaint::on_bookMarkIt_clicked()
{
    if (!QFile(filepath).exists()) {
        QString mess = "File: " + currentFile + "' not exists Or not saved";
        messageEngine(mess, "Info");
    } else {
        bookmarks bookMarks;
        bookMarks.callBookMarkDialog(this, filepath);
    }
}

void corepaint::on_cut_clicked()
{
    if (ImageArea *imageArea = getCurrentImageArea())
        imageArea->cutImage();
}

void corepaint::on_copy_clicked()
{
    if (ImageArea *imageArea = getCurrentImageArea())
        imageArea->copyImage();
}

void corepaint::on_past_clicked()
{
    if (ImageArea *imageArea = getCurrentImageArea())
        imageArea->pasteImage();
}

void corepaint::on_openshots_clicked()
{
    CoreBox *cBox = qobject_cast<CoreBox*>(qApp->activeWindow());
    cBox->tabEngine(CoreFM, sm.getSCSaveLocation());
}

void corepaint::undo() {
    mUndoStackGroup->undo();
}

void corepaint::redo() {
    mUndoStackGroup->redo();
}

void corepaint::on_delet_clicked()
{
    if (ImageArea *imageArea = getCurrentImageArea())
       imageArea->clearBackground();
}
