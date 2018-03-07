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

#include "corefm.h"
#include "ui_corefm.h"
#include "mymodel.h"
#include "progressdlg.h"
#include "fileutils.h"
#include "applicationdialog.h"

#include <QtGui>
#include <QInputDialog>
#include <QApplication>
#include <QMenu>
#include <sys/vfs.h>
#include <fcntl.h>
#include <QDebug>
#include <QSettings>
#include <QDateTime>
#include <QtConcurrent/QtConcurrent>

#include "bookmarks.h"
#include "bookmarkdialog.h"
#include "../coreimage/coreimage.h"
#include "../corepad/corepad.h"
#include "coreplayer/coreplayer.h"
#include "corepaint/corepaint.h"
#include "search/search.h"

corefm::corefm(QWidget *parent) :QWidget(parent),ui(new Ui::corefm)
{
    qDebug() << "corefm opening";
    ui->setupUi(this);

    QIcon::setThemeName(sm.getThemeName());

    // Create mime utils
    mimeUtils = new MimeUtils(this);
//    QString tmp = "/.local/share/applications/mimeapps.ui->viewlist";
//    QString name = sett->value("Default-Mime-Apps-File", tmp).toString();

    QString name = sm.getMimeFilePath();
    mimeUtils->setDefaultsFileName(name);

    // Create filesystem model
    bool realMime = sm.getIsRealMimeType();

    QString startP;

    if (sm.getStartupPath() == "") {
        startP = QDir::homePath();
    } else {
        startP = sm.getStartupPath();
    }
    startPath = startP;

    modelList = new myModel(realMime, mimeUtils);

    tabs = new tabBar(modelList->folderIcons);
    tabs->setTabsClosable(true);
    connect(tabs, SIGNAL(tabCloseRequested(int)), tabs, SLOT(closeTab(int)));
    tabs->setStyleSheet("QTabBar::tab:!selected{background-color:#353536;color:#B4B4B4;}"
                        "QTabBar::tab:selected{background-color: #2E2E2E;font-weight: bold;color:#ffffff;}"
                        "QTabBar::close-button {image: url(:/icons/close_w.svg);subcontrol-position: right;}"
                        "QTabBar::tab{text-align:left;background-color: #136ba2;border-style: 1px rgb(67, 67, 67);height: 30px;color: #136ba2;padding: 0px 5px 0px 5px;}"
                        "QTabBar{background-color: #353536;height: 30px;}");

    modelTree = new mainTreeFilterProxyModel();
    modelTree->setSourceModel(modelList);
    modelTree->setSortCaseSensitivity(Qt::CaseInsensitive);

    tree = new QTreeView();
    ui->lp->addWidget(tree);
    tree->setVisible(false);
    tree->setHeaderHidden(true);
    tree->setUniformRowHeights(true);
    tree->setModel(modelTree);
    tree->hideColumn(1);
    tree->hideColumn(2);
    tree->hideColumn(3);
    tree->hideColumn(4);

    modelView = new viewsSortProxyModel();
    modelView->setSourceModel(modelList);
    modelView->setSortCaseSensitivity(Qt::CaseInsensitive);

    ui->viewlist->setModel(modelView);
    ui->viewlist->setFocusPolicy(Qt::NoFocus);
    listSelectionModel = ui->viewlist->selectionModel();
    ui->viewlist->setContextMenuPolicy(Qt::CustomContextMenu);

    ui->viewtree->setModel(modelView);
    ui->viewtree->setSelectionModel(listSelectionModel);
    ui->viewtree->setFocusPolicy(Qt::NoFocus);
    int i = ui->navigationBar->sizeHint().width();
    ui->viewtree->setColumnWidth(0,i);
    ui->viewtree->setContextMenuPolicy(Qt::CustomContextMenu);

    treeSelectionModel = tree->selectionModel();
    connect(treeSelectionModel, SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this, SLOT(treeSelectionChanged(QModelIndex,QModelIndex)));
    tree->setCurrentIndex(modelTree->mapFromSource(modelList->index(startPath)));
    tree->scrollTo(tree->currentIndex());

    loadSettings();
    lateStart();
    shotcuts();

    connect(ui->zoomin,SIGNAL(pressed()),this,SLOT(zoomInAction()));
    connect(ui->zoomout,SIGNAL(pressed()),this,SLOT(zoomOutAction()));
    connect(ui->detaile, SIGNAL(clicked(bool)), this, SLOT(on_detaile_clicked(bool)));
    connect(ui->icon, SIGNAL(clicked(bool)), this, SLOT(on_icon_clicked(bool)));

    ui->paste->setVisible(0);
    ui->mk->addWidget(tabs);
    ui->TrashFrame->setVisible(0);
//    toggleSortBy(sortNameAct);
    on_showHidden_clicked(0);
    ui->emptyTrash->setVisible(0);
    ui->showthumb->setVisible(0);

    ui->newTab->setDefaultAction(ui->actionNewTab);
    ui->copy->setDefaultAction(ui->actionCopy);
    ui->cut->setDefaultAction(ui->actionCut);
    ui->paste->setDefaultAction(ui->actionPaste);
    ui->properties->setDefaultAction(ui->actionProperties);
    ui->deleteit->setDefaultAction(ui->actionDelete);
    ui->up->setDefaultAction(ui->actionUp);
    ui->back->setDefaultAction(ui->actionBack);
    ui->terminal->setDefaultAction(ui->actionTerminal);
    ui->refresh->setDefaultAction(ui->actionRefresh);
    ui->newfolder->setDefaultAction(ui->actionNewFolder);
    ui->newtext->setDefaultAction(ui->actionNewTextFile);

//    watcher = new QFileSystemWatcher(this);
//    connect(watcher, SIGNAL(directoryChanged(QString)), this, SLOT(reloadList()));
}

corefm::~corefm()
{
    delete ui;
}

void corefm::shotcuts(){
    QShortcut* shortcut;

    shortcut = new QShortcut(QKeySequence(Qt::Key_Delete), this);
    connect(shortcut, &QShortcut::activated, this, &corefm::on_actionDelete_triggered);
    shortcut = new QShortcut(QKeySequence(Qt::Key_T), this);
    connect(shortcut, &QShortcut::activated, this, &corefm::on_actionTerminal_triggered);
    shortcut = new QShortcut(QKeySequence(Qt::Key_Backspace), this);
    connect(shortcut, &QShortcut::activated, this, &corefm::on_actionUp_triggered);
    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_A), this);
    connect(shortcut, &QShortcut::activated, this, &corefm::on_actionSelectAll_triggered);
    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_F), this);
    connect(shortcut, &QShortcut::activated, this, &corefm::on_searchHere_clicked);
    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_D), this);
    connect(shortcut, &QShortcut::activated, this, &corefm::on_actionTrash_it_triggered);
    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_X), this);
    connect(shortcut, &QShortcut::activated, this, &corefm::on_actionCut_triggered);
    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_C), this);
    connect(shortcut, &QShortcut::activated, this, &corefm::on_actionCopy_triggered);
    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_V), this);
    connect(shortcut, &QShortcut::activated, this, &corefm::on_actionPaste_triggered);
    shortcut = new QShortcut(QKeySequence(Qt::CTRL +Qt::Key_Shift + Qt::Key_T), this);
    connect(shortcut, &QShortcut::activated, this, &corefm::on_actionNewTextFile_triggered);
    shortcut = new QShortcut(QKeySequence(Qt::CTRL +Qt::Key_Shift + Qt::Key_N), this);
    connect(shortcut, &QShortcut::activated, this, &corefm::on_actionNewFolder_triggered);
    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Space), this);
    connect(shortcut, &QShortcut::activated, this, &corefm::on_actionNewTab_triggered);
    shortcut = new QShortcut(QKeySequence(Qt::Key_R), this);
    connect(shortcut, &QShortcut::activated, this, &corefm::on_actionRefresh_triggered);
    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_B), this);
    connect(shortcut, &QShortcut::activated, this, &corefm::on_SBookMarkIt_clicked);
}

void corefm::lateStart()
{
  // Configure tree view
  tree->setDragDropMode(QAbstractItemView::DragDrop);
  tree->setDefaultDropAction(Qt::MoveAction);
  tree->setDropIndicatorShown(true);
  tree->setEditTriggers(QAbstractItemView::EditKeyPressed | QAbstractItemView::SelectedClicked);

  ui->viewlist->setFocus();

  // Watch for mounts
  int fd = open("/proc/self/mounts", O_RDONLY, 0);
  notify = new QSocketNotifier(fd, QSocketNotifier::Write);

  // Clipboard configuration
  progress = 0;
  clipboardChanged();

  // Completer configuration
  customComplete = new myCompleter;
  customComplete->setModel(modelTree);
  customComplete->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
  customComplete->setMaxVisibleItems(10);
  ui->pathEdit->setCompleter(customComplete);

  // Tabs configuration
  tabs->setDrawBase(0);
  tabs->setExpanding(0);

  // Connect mouse clicks in views
//  if (sett->value("singleClick").toInt() == 1) {
//    connect(ui->viewlist, SIGNAL(clicked(QModelIndex)),this, SLOT(listItemClicked(QModelIndex)));
//    connect(ui->viewtree, SIGNAL(clicked(QModelIndex)),this, SLOT(listItemClicked(QModelIndex)));
//  }
//  if (sett->value("singleClick").toInt() == 2) {
//    connect(ui->viewlist, SIGNAL(clicked(QModelIndex)),this, SLOT(listDoubleClicked(QModelIndex)));
//    connect(ui->viewtree, SIGNAL(clicked(QModelIndex)),this, SLOT(listDoubleClicked(QModelIndex)));
//  }

  // Connect ui->viewlist view
  connect(ui->viewlist, SIGNAL(activated(QModelIndex)),this, SLOT(listDoubleClicked(QModelIndex)));

  // Connect path edit
  connect(ui->pathEdit, SIGNAL(activated(QString)),this, SLOT(pathEditChanged(QString)));
  connect(customComplete, SIGNAL(activated(QString)),this, SLOT(pathEditChanged(QString)));
  connect(ui->pathEdit->lineEdit(), SIGNAL(cursorPositionChanged(int,int)),this, SLOT(addressChanged(int,int)));

  // Connect selection
  connect(QApplication::clipboard(), SIGNAL(changed(QClipboard::Mode)),this, SLOT(clipboardChanged()));
  connect(ui->viewtree,SIGNAL(activated(QModelIndex)),this, SLOT(listDoubleClicked(QModelIndex)));
  connect(listSelectionModel,SIGNAL(selectionChanged(const QItemSelection, const QItemSelection)),
          this, SLOT(listSelectionChanged(const QItemSelection,const QItemSelection)));

  // Connect copy progress
  connect(this, SIGNAL(copyProgressFinished(int,QStringList)),this, SLOT(progressFinished(int,QStringList)));

  // Conect ui->viewlist model
  connect(modelList,SIGNAL(dragDropPaste(const QMimeData *, QString, myModel::DragMode)),
          this,SLOT(dragLauncher(const QMimeData *, QString, myModel::DragMode)));

  // Connect tabs
  connect(tabs, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));
  connect(tabs, SIGNAL(dragDropTab(const QMimeData *, QString, QStringList)),this, SLOT(pasteLauncher(const QMimeData *, QString, QStringList)));
  connect(ui->viewlist, SIGNAL(pressed(QModelIndex)),this, SLOT(listItemPressed(QModelIndex)));
  connect(ui->viewtree, SIGNAL(pressed(QModelIndex)),this, SLOT(listItemPressed(QModelIndex)));
  connect(modelList, SIGNAL(thumbUpdate(QModelIndex)),this, SLOT(thumbUpdate(QModelIndex)));

  qApp->setKeyboardInputInterval(1000);

  // Read defaults
  QTimer::singleShot(100, mimeUtils, SLOT(generateDefaults()));
  on_actionRefresh_triggered();
}

/**
 * @brief Loads application settings
 */
void corefm::loadSettings()
{
    modelList->setRealMimeTypes(sm.getIsRealMimeType());

    zoom = sm.getZoomValue();
    zoomTree = sm.getZoomTreeValue();
    zoomList = sm.getZoomListValue();
    zoomDetail = sm.getZoomDetailValue();
    ui->viewtree->setIconSize(QSize(zoomDetail, zoomDetail));
    tree->setIconSize(QSize(zoomTree, zoomTree));

    ui->showthumb->setChecked(sm.getIsShowThumb());
    ui->Tools->setChecked(sm.getShowToolbox());
    ui->tools->setVisible(sm.getShowToolbox());

    // Load view mode
    if (sm.getViewMode() == true) {
        on_icon_clicked(1);
        ui->icon->setChecked(1);
    }
    else if (sm.getViewMode() == false) {
        on_detaile_clicked(1);
        ui->detaile->setChecked(1);
    }

    // Load terminal command
    term = sm.getTerminal();

    currentSortColumn = sm.getSortColumn();//sett->value("Sort-Column", 0).toInt();
    currentSortOrder = (Qt::SortOrder)sm.getSortOrder();// sett->value("Sort-Order", 0).toInt();

//    ui->viewtree->setSortingEnabled(1);
//    switch (currentSortColumn) {
//      case 0 : setSortColumn(sortNameAct); break;
//      case 1 : setSortColumn(sortSizeAct); break;
//      case 3 : setSortColumn(sortDateAct); break;
//    }
//    setSortOrder(currentSortOrder);
//    modelView->sort(currentSortColumn, currentSortOrder);
}

/**
 * @brief Writes settings into config file
 */
void corefm::writeSettings() {
    sm.setZoomValue(zoom);
    sm.setZoomTreeValue(zoomTree);
    sm.setZoomListValue(zoomList);
    sm.setZoomDetailValue(zoomDetail);
    sm.setSortColumn(currentSortColumn);
    sm.setSortOrder(currentSortOrder);
    sm.setIsShowThumb(ui->showthumb->isChecked());
    sm.setViewMode(ui->icon->isChecked());
    sm.setShowToolbox(ui->tools->isVisible());
    sm.setIsRealMimeType(modelList->isRealMimeTypes());
}

void corefm::closeEvent(QCloseEvent *event)
{
    // Save settings
    event->ignore();

    writeSettings();
    if (tabs->count() == 0) {
        saveToRecent("CoreFM", ui->pathEdit->currentText());
    }
    else if (tabs->count() > 0) {
        for (int i = 0; i < tabs->count(); i++) {
            tabs->setCurrentIndex(i);
            qDebug() << ui->pathEdit->currentText() << tabs->currentIndex();
            saveToRecent("CoreFM", ui->pathEdit->currentText());
        }
    }

  // If deamon, ignore event
//  if (isDaemon) {
//    this->setVisible(0);
//    //startDaemon();
//    customComplete->setModel(0);
//    modelList->refresh();
//    tabs->setCurrentIndex(0);
//    tree->setCurrentIndex(modelTree->mapFromSource(modelList->index(startPath)));
//    tree->scrollTo(tree->currentIndex());
//    customComplete->setModel(modelTree);
//    event->ignore();
//  } else {
    modelList->cacheInfo();
    event->accept();
  //}
}


void corefm::treeSelectionChanged(QModelIndex current, QModelIndex previous)
{
    Q_UNUSED(previous);
    QFileInfo name = modelList->fileInfo(modelTree->mapToSource(current));
    if(!name.exists()) return;

    curIndex = name;

    if(tree->hasFocus() && QApplication::mouseButtons() == Qt::MidButton)
    {
        listItemPressed(modelView->mapFromSource(modelList->index(name.filePath())));
        tabs->setCurrentIndex(tabs->count() - 1);
        if(currentView == 2) ui->viewtree->setFocus(Qt::TabFocusReason);
        else ui->viewlist->setFocus(Qt::TabFocusReason);
    }

    if(curIndex.filePath() != ui->pathEdit->itemText(0))
    {
        if(tabs->count()) tabs->addHistory(curIndex.filePath());
        ui->pathEdit->insertItem(0,curIndex.filePath());
        ui->pathEdit->setCurrentIndex(0);
    }

    if(modelList->setRootPath(name.filePath())) modelView->invalidate();

    QModelIndex baseIndex = modelView->mapFromSource(modelList->index(name.filePath()));

    if(currentView == 2) ui->viewtree->setRootIndex(baseIndex);
    else ui->viewlist->setRootIndex(baseIndex);

    if(tabs->count())
    {
        tabs->setTabText(tabs->currentIndex(),curIndex.fileName());
        tabs->setTabData(tabs->currentIndex(),curIndex.filePath());
        tabs->setIcon(tabs->currentIndex());
    }

    if(backIndex.isValid())
    {
        listSelectionModel->setCurrentIndex(modelView->mapFromSource(backIndex),QItemSelectionModel::ClearAndSelect);
        if(currentView == 2) ui->viewtree->scrollTo(modelView->mapFromSource(backIndex));
        else ui->viewlist->scrollTo(modelView->mapFromSource(backIndex));
    }
    else
    {
        listSelectionModel->blockSignals(1);
        listSelectionModel->clear();
    }

    listSelectionModel->blockSignals(0);
    QTimer::singleShot(30,this,SLOT(dirLoaded()));
}

void corefm::dirLoaded()
{
    if(backIndex.isValid()){
        backIndex = QModelIndex();
        return;
    }

    QModelIndexList items;

    for(int x = 0; x < modelList->rowCount(modelList->index(ui->pathEdit->currentText())); ++x)
        items.append(modelList->index(x,0,modelList->index(ui->pathEdit->currentText())));


    ui->totalitem->setText("Total : " + QString("%1 items").arg(items.count()));
    ui->selecteditem->setText("Selected : 0 items");

    if(ui->showthumb->isChecked()) QtConcurrent::run(modelList,&myModel::loadThumbs,items);
}

void corefm::thumbUpdate(QModelIndex index)
{
    if(currentView == 2) ui->viewtree->update(modelView->mapFromSource(index));
    else ui->viewlist->update(modelView->mapFromSource(index));
}

void corefm::listSelectionChanged(const QItemSelection selected, const QItemSelection deselected)
{
    Q_UNUSED(selected);
    Q_UNUSED(deselected);
    QModelIndexList items;

    if(listSelectionModel->selectedRows(0).count()) items = listSelectionModel->selectedRows(0);
    else items = listSelectionModel->selectedIndexes();

    selcitem = items.count();

    ui->selecteditem->clear();
    if(selcitem == 0)
    {
        curIndex = ui->pathEdit->itemText(0);
        return;
    }

    curIndex = modelList->filePath(modelView->mapToSource(listSelectionModel->currentIndex()));

    if(selcitem==1){
       selcitempath = curIndex.absoluteFilePath();
    }else{
       selcitempath = ui->pathEdit->itemText(0);
    }

    ui->name->setText(curIndex.fileName());
    ui->selecteditem->setText("Selected : " + QString("%1 items").arg(items.count()));

    //for multipal file size
//    QModelIndexList selList;
//    if(focusWidget() == ui->viewlist || focusWidget() == ui->viewtree){
//        if(listSelectionModel->selectedRows(0).count()) selList = listSelectionModel->selectedRows(0);
//        else selList = listSelectionModel->selectedIndexes();
//    }

//    if(selList.count() == 0) selList << modelView->mapFromSource(modelList->index(ui->pathEdit->currentText()));

//    QStringList paths;

//    foreach(QModelIndex item, selList)
//        paths.append(modelList->filePath(modelView->mapToSource(item)));

//    qDebug()<< paths;
//    qDebug()<< modelList;
}

void corefm::listItemClicked(QModelIndex current)
{
    if(modelList->filePath(modelView->mapToSource(current)) == ui->pathEdit->currentText()) return;

    Qt::KeyboardModifiers mods = QApplication::keyboardModifiers();
    if(mods == Qt::ControlModifier || mods == Qt::ShiftModifier) return;
    if(modelList->isDir(modelView->mapToSource(current)))
        tree->setCurrentIndex(modelTree->mapFromSource(modelView->mapToSource(current)));
}

void corefm::listItemPressed(QModelIndex current)
{
    //middle-click -> open new tab
    //ctrl+middle-click -> open new instance

    if(QApplication::mouseButtons() == Qt::MidButton){
        if(modelList->isDir(modelView->mapToSource(current)))
        {
            if(QApplication::keyboardModifiers() == Qt::ControlModifier)
                on_actionOpen_triggered();
            else
                addTab(modelList->filePath(modelView->mapToSource(current)));
        }
        else{
            on_actionOpen_triggered();
        }
    }
}

int corefm::addTab(QString path)
{
    if (tabs->count() < 10) {
        if(tabs->count() == 0) tabs->addNewTab(ui->pathEdit->currentText(),currentView);
        return tabs->addNewTab(path,currentView);
    } else {
        messageEngine("Maximum Number of tabs reached.", "Warning");
    }
    return -1;
}

void corefm::tabChanged(int index)
{
    if(tabs->count() == 0) return;

    ui->pathEdit->clear();
    ui->pathEdit->addItems(*tabs->getHistory(index));

    int type = tabs->getType(index);
    if(currentView != type)
    {
        if(type == 2) ui->detaile->setChecked(1);
        else ui->detaile->setChecked(0);

        if(type == 1) ui->icon->setChecked(1);
        else ui->icon->setChecked(0);

        on_detaile_clicked(true);
    }

    if(!tabs->tabData(index).toString().isEmpty())
        tree->setCurrentIndex(modelTree->mapFromSource(modelList->index(tabs->tabData(index).toString())));
}

/**
 * @brief Doubleclick on icon/launcher
 * @param current
 */
void corefm::listDoubleClicked(QModelIndex current) {
  Qt::KeyboardModifiers mods = QApplication::keyboardModifiers();
  if (mods == Qt::ControlModifier || mods == Qt::ShiftModifier) {
    return;
  }
  if (modelList->isDir(modelView->mapToSource(current))) {
    QModelIndex i = modelView->mapToSource(current);
    tree->setCurrentIndex(modelTree->mapFromSource(i));
  } else {
    executeFile(current, 0);
  }
}

/**
 * @brief Reaction for change of path edit (location edit)
 * @param path
 */
void corefm::pathEditChanged(QString path) {
  QString info = path;
  if (!QFileInfo(path).exists()) return;
  info.replace("~",QDir::homePath());
  tree->setCurrentIndex(modelTree->mapFromSource(modelList->index(info)));
}

/**
 * @brief Reaction for change of clippboard content
 */
void corefm::clipboardChanged()
{
  if (QApplication::clipboard()->mimeData()->hasUrls()) {
    ui->actionPaste->setEnabled(1);
    ui->paste->setVisible(1);
  } else {
    modelList->clearCutItems();
    ui->actionPaste->setEnabled(0);
    ui->paste->setVisible(0);
  }
}

/**
 * @brief Drags data to the new location
 * @param data data to be pasted
 * @param newPath path of new location
 * @param dragMode mode of dragging
 */
void corefm::dragLauncher(const QMimeData *data, const QString &newPath,
                              myModel::DragMode dragMode) {

  // Retrieve urls (paths) of data
  QList<QUrl> files = data->urls();

  // If drag mode is unknown then ask what to do
  if (dragMode == myModel::DM_UNKNOWN) {
    QMessageBox box;
    box.setWindowTitle(tr("What do you want to do?"));
    QAbstractButton *move = box.addButton(tr("Move here"), QMessageBox::ActionRole);
    QAbstractButton *copy = box.addButton(tr("Copy here"), QMessageBox::ActionRole);
    QAbstractButton *link = box.addButton(tr("Link here"), QMessageBox::ActionRole);
    QAbstractButton *canc = box.addButton(QMessageBox::Cancel);
    box.exec();
    if (box.clickedButton() == move) {
      dragMode = myModel::DM_MOVE;
    } else if (box.clickedButton() == copy) {
      dragMode = myModel::DM_COPY;
    } else if (box.clickedButton() == link) {
      dragMode = myModel::DM_LINK;
    } else if (box.clickedButton() == canc) {
      return;
    }
  }

  // If moving is enabled, cut files from the original location
  QStringList cutList;
  if (dragMode == myModel::DM_MOVE) {
    foreach (QUrl item, files) {
      cutList.append(item.path());
    }
  }

  // Paste launcher (this method has to be called instead of that with 'data'
  // parameter, because that 'data' can timeout)
  pasteLauncher(files, newPath, cutList, dragMode == myModel::DM_LINK);
}

/**
 * @brief Pastes data to the new location
 * @param data data to be pasted
 * @param newPath path of new location
 * @param cutList ui->viewlist of items to remove
 */
void corefm::pasteLauncher(const QMimeData *data, const QString &newPath,
                               const QStringList &cutList) {
  QList<QUrl> files = data->urls();
  pasteLauncher(files, newPath, cutList);
}

/**
 * @brief Pastes files to the new path
 * @param files ui->viewlist of files
 * @param newPath new path
 * @param cutList files to remove from original path
 * @param link true if link should be created (default value = false)
 */
void corefm::pasteLauncher(const QList<QUrl> &files, const QString &newPath,
                               const QStringList &cutList, bool link) {

  // File no longer exists?
  if (!QFile(files.at(0).path()).exists()) {
    QString msg = tr("File '%1' no longer exists!").arg(files.at(0).path());
    QMessageBox::information(this, tr("No paste for you!"), msg);
    ui->actionPaste->setEnabled(0);
    ui->paste->setVisible(0);
    return;
  }

  // Temporary variables
  int replace = 0;
  QStringList completeList;
  QString baseName = QFileInfo(files.at(0).toLocalFile()).path();

  // Only if not in same directory, otherwise we will do 'Copy(x) of'
  if (newPath != baseName) {

    foreach (QUrl file, files) {

      // Merge or replace?
      QFileInfo temp(file.toLocalFile());

      if (temp.isDir() && QFileInfo(newPath + QDir::separator() + temp.fileName()).exists()) {
        QString msg = QString("<b>%1</b><p>Already exists!<p>What do you want to do?").arg(newPath + QDir::separator() + temp.fileName());
        QMessageBox message(QMessageBox::Question, tr("Existing folder"), msg, QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        message.button(QMessageBox::Yes)->setText(tr("Merge"));
        message.button(QMessageBox::No)->setText(tr("Replace"));

        int merge = message.exec();
        if (merge == QMessageBox::Cancel) return;
        if (merge == QMessageBox::Yes) {
          FileUtils::recurseFolder(temp.filePath(), temp.fileName(), &completeList);
        }
        else {
          FileUtils::removeRecurse(newPath, temp.fileName());
        }
      }
      else completeList.append(temp.fileName());
    }

    // Ask whether replace files if files with same name already exist in
    // destination directory
    foreach (QString file, completeList) {
      QFileInfo temp(newPath + QDir::separator() + file);
      if (temp.exists()) {
        QFileInfo orig(baseName + QDir::separator() + file);
        if (replace != QMessageBox::YesToAll && replace != QMessageBox::NoToAll) {
          // TODO: error dispalys only at once
          replace = showReplaceMsgBox(temp, orig);
        }
        if (replace == QMessageBox::Cancel) {
          return;
        }
        if (replace == QMessageBox::Yes || replace == QMessageBox::YesToAll) {
          QFile(temp.filePath()).remove();
        }
      }
    }
  }

  // If only links should be created, create them and exit
  if (link) {
    linkFiles(files, newPath);
    return;
  }

  // Copy/move files
  QString title = cutList.count() == 0 ? tr("Copying...") : tr("Moving...");
  progress = new myProgressDialog(title);
  connect(this, SIGNAL(updateCopyProgress(qint64, qint64, QString)), progress, SLOT(update(qint64, qint64, QString)));
  listSelectionModel->clear();
  QtConcurrent::run(this, &corefm::pasteFiles, files, newPath, cutList);
}

/**
 * @brief Pastes ui->viewlist of files/dirs into new path
 * @param files ui->viewlist of files
 * @param newPath new (destination) path
 * @param cutList ui->viewlist of files that are going to be removed from source path
 * @return true if operation was successfull
 */
bool corefm::pasteFiles(const QList<QUrl> &files, const QString &newPath,const QStringList &cutList)
{
    // Temporary variables
    bool ok = true;
    QStringList newFiles;

    // Quit if folder not writable
    if (!QFileInfo(newPath).isWritable()
        || newPath == QDir(files.at(0).toLocalFile()).path()) {
      emit copyProgressFinished(1, newFiles);
      return 0;
    }

    // Get total size in bytes
    qint64 total = FileUtils::totalSize(files);

    // Check available space on destination before we start
    struct statfs info;
    statfs(newPath.toLocal8Bit(), &info);
    if ((qint64) info.f_bavail * info.f_bsize < total) {

      // If it is a cut/move on the same device it doesn't matter
      if (cutList.count()) {
        qint64 driveSize = (qint64) info.f_bavail*info.f_bsize;
        statfs(files.at(0).path().toLocal8Bit(),&info);

        // Same device?
        if ((qint64) info.f_bavail*info.f_bsize != driveSize) {
          emit copyProgressFinished(2, newFiles);
          return 0;
        }
      } else {
        emit copyProgressFinished(2, newFiles);
        return 0;
      }
    }

    // Main loop
    for (int i = 0; i < files.count(); ++i) {

      // Canceled ?
      if (progress->result() == 1) {
        emit copyProgressFinished(0, newFiles);
        return 1;
      }

      // Destination file name and url
      QFileInfo temp(files.at(i).toLocalFile());
      QString destName = temp.fileName();
      QString destUrl = newPath + QDir::separator() + destName;

      // Only do 'Copy(x) of' if same folder
      if (temp.path() == newPath) {
        int num = 1;
        while (QFile(destUrl).exists()) {
          destName = QString("Copy (%1) of %2").arg(num).arg(temp.fileName());
          destUrl = newPath + QDir::separator() + destName;
          num++;
        }
      }

      // If destination file does not exist and is directory
      QFileInfo dName(destUrl);
      if (!dName.exists() || dName.isDir()) {

        // Keep a ui->viewlist of new files so we can select them later
        newFiles.append(destUrl);

        // Cut action
        if (cutList.contains(temp.filePath())) {

          // Files or directories
          if (temp.isFile()) {

            // NOTE: Rename will fail if across different filesystem
            /*QFSFileEngine*/ QFile file(temp.filePath());
            if (!file.rename(destUrl))	{
              ok = cutCopyFile(temp.filePath(), destUrl, total, true);
            }
          } else {
            ok = QFile(temp.filePath()).rename(destUrl);

            // File exists or move folder between different filesystems, so use
            // copy/remove method
            if (!ok) {
              if (temp.isDir()) {
                ok = true;
                copyFolder(temp.filePath(), destUrl, total, true);
                modelList->clearCutItems();
              }
              // File already exists, don't do anything
            }
          }
        } else {
          if (temp.isDir()) {
            copyFolder(temp.filePath(),destUrl,total,false);
          } else {
            ok = cutCopyFile(temp.filePath(), destUrl, total, false);
          }
        }
      }
    }
    qDebug()<< "pasteFiles";

    // Finished
    emit copyProgressFinished(0, newFiles);
    return 1;
}

/**
 * @brief Copies source directory to destination directory
 * @param srcFolder location of source directory
 * @param dstFolder location of destination directory
 * @param total total copy size
 * @param cut true/false if source directory is going to be moved/copied
 * @return true if copy was successfull
 */
bool corefm::copyFolder(const QString &srcFolder, const QString &dstFolder,qint64 total, bool cut)
{
    // Temporary variables
    QDir srcDir(srcFolder);
    QDir dstDir(QFileInfo(dstFolder).path());
    QStringList files;
    bool ok = true;

    // Name of destination directory
    QString folderName = QFileInfo(dstFolder).fileName();

    // Id destination location does not exist, create it
    if (!QFileInfo(dstFolder).exists()) {
      dstDir.mkdir(folderName);
    }
    dstDir = QDir(dstFolder);

    // Get files in source directory
    files = srcDir.entryList(QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden);

    // Copy each file
    for (int i = 0; i < files.count(); i++) {
      QString srcName = srcDir.path() + QDir::separator() + files[i];
      QString dstName = dstDir.path() + QDir::separator() + files[i];

      // Don't remove source folder if all files not cut
      if (!cutCopyFile(srcName, dstName, total, cut)) ok = false;

      // Cancelled
      if (progress->result() == 1) return 0;
    }

    // Get directories in source directory
    files.clear();
    files = srcDir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::Hidden);

    // Copy each directory
    for (int i = 0; i < files.count(); i++) {
      if (progress->result() == 1) {
        return 0;
      }
      QString srcName = srcDir.path() + QDir::separator() + files[i];
      QString dstName = dstDir.path() + QDir::separator() + files[i];
      copyFolder(srcName, dstName, total, cut);
    }

    // Remove source folder if all files moved ok
    if (cut && ok) {
      srcDir.rmdir(srcFolder);
    }
    qDebug()<< "copyFolder";
    return ok;
}

/**
 * @brief Copies or moves file
 * @param src location of source file
 * @param dst location of destination file
 * @param totalSize total copy size
 * @param cut true/false if source file is going to be moved/copied
 * @return true if copy was successfull
 */
bool corefm::cutCopyFile(const QString &src, QString dst, qint64 totalSize,bool cut)
{
    // Create files with given locations
    QFile srcFile(src);
    QFile dstFile(dst);

    // Destination file already exists, exit
    if (dstFile.exists()) return 1;

    // If destination location is too long make it shorter
    if (dst.length() > 50) dst = "/.../" + dst.split(QDir::separator()).last();

    // Open source and destination files
    srcFile.open(QFile::ReadOnly);
    dstFile.open(QFile::WriteOnly);

    // Determine buffer size, calculate size of file and number of steps
    char block[4096];
    qint64 total = srcFile.size();
    qint64 steps = total >> 7; // shift right 7, same as divide 128, much faster
    qint64 interTotal = 0;

    // Copy blocks
    while (!srcFile.atEnd()) {
      if (progress->result() == 1) break; // cancelled
      qint64 inBytes = srcFile.read(block, sizeof(block));
      dstFile.write(block, inBytes);
      interTotal += inBytes;
      if (interTotal > steps) {
        emit updateCopyProgress(interTotal, totalSize, dst);
        interTotal = 0;
      }
    }

    // Update copy progress
    emit updateCopyProgress(interTotal, totalSize, dst);

    dstFile.close();
    srcFile.close();

    if (dstFile.size() != total) return 0;
    if (cut) srcFile.remove();  // if file is cut remove the source
    qDebug()<< "cutCopyFile";
    return 1;
}

/**
 * @brief Creates symbolic links to files
 * @param files
 * @param newPath
 * @return true if link creation was successfull
 */
bool corefm::linkFiles(const QList<QUrl> &files, const QString &newPath)
{
    // Quit if folder not writable
    if (!QFileInfo(newPath).isWritable()
        || newPath == QDir(files.at(0).toLocalFile()).path()) {
      return false;
    }

    // TODO: even if symlinks are small we have to make sure that we have space
    // available for links

    // Main loop
    for (int i = 0; i < files.count(); ++i) {

      // Choose destination file name and url
      QFile file(files.at(i).toLocalFile());
      QFileInfo temp(file);
      QString destName = temp.fileName();
      QString destUrl = newPath + QDir::separator() + destName;

      // Only do 'Link(x) of' if same folder
      if (temp.path() == newPath) {
        int num = 1;
        while (QFile(destUrl).exists()) {
          destName = QString("Link (%1) of %2").arg(num).arg(temp.fileName());
          destUrl = newPath + QDir::separator() + destName;
          num++;
        }
      }

      // If file does not exists then create link
      QFileInfo dName(destUrl);
      if (!dName.exists()) {
        file.link(destUrl);
      }
    }
    qDebug()<< "linkFiles";
    return true;
}

/**
 * @brief Asks user whether replace file 'f1' with another file 'f2'
 * @param f1 file to be replaced with f2
 * @param f2 file to replace f1
 * @return result
 */
int corefm::showReplaceMsgBox(const QFileInfo &f1, const QFileInfo &f2)
{
    // Create message
    QString t = tr("Do you want to replace:<p><b>%1</p><p>Modified: %2<br>"
                   "Size: %3 bytes</p><p>with:<p><b>%4</p><p>Modified: %5"
                   "<br>Size: %6 bytes</p>");

    // Populate message with data
    t = t.arg(f1.filePath()).arg(f1.lastModified().toString()).arg(f1.size())
         .arg(f2.filePath()).arg(f2.lastModified().toString()).arg(f2.size());

    // Show message
    return QMessageBox::question(0, tr("Replace"), t, QMessageBox::Yes
                                 | QMessageBox::YesToAll | QMessageBox::No
                                 | QMessageBox::NoToAll | QMessageBox::Cancel);
}

void corefm::progressFinished(int ret,QStringList newFiles)
{
    if(progress != 0)
    {
        progress->close();
        delete progress;
        progress = 0;
    }

    if(newFiles.count())
    {
        disconnect(listSelectionModel,SIGNAL(selectionChanged(const QItemSelection, const QItemSelection)),this,SLOT(listSelectionChanged(const QItemSelection, const QItemSelection)));

        qApp->processEvents();              //make sure notifier has added new files to the model

        if(QFileInfo(newFiles.first()).path() == ui->pathEdit->currentText())       //highlight new files if visible
        {
            foreach(QString item, newFiles)
                listSelectionModel->select(modelView->mapFromSource(modelList->index(item)),QItemSelectionModel::Select);
        }

        connect(listSelectionModel,SIGNAL(selectionChanged(const QItemSelection, const QItemSelection)),this,SLOT(listSelectionChanged(const QItemSelection, const QItemSelection)));
        curIndex.setFile(newFiles.first());

        if(currentView == 2) ui->viewtree->scrollTo(modelView->mapFromSource(modelList->index(newFiles.first())),QAbstractItemView::EnsureVisible);
        else ui->viewlist->scrollTo(modelView->mapFromSource(modelList->index(newFiles.first())),QAbstractItemView::EnsureVisible);

        if(QFile(QDir::tempPath() + "/corefm.temp").exists()) QApplication::clipboard()->clear();

        clearCutItems();
    }

    if(ret == 1) QMessageBox::information(this,tr("Failed"),tr("Paste failed...do you have write permissions?"));
    if(ret == 2) QMessageBox::warning(this,tr("Too big!"),tr("There is not enough space on the destination drive!"));
}


/**
 * @brief Creates menu for opening file in selected application
 * @return menu
 */
QMenu* corefm::createOpenWithMenu() {
    // Add open with functionality ...
    QMenu *openMenu = new QMenu(tr("Open with"));

    // Select action
    QAction *selectAppAct = new QAction(tr("Select..."), openMenu);
  //  selectAppAct->setStatusTip(tr("Select application for opening the file"));
    //selectAppAct->setIcon(actionIcons->at(18));
    connect(selectAppAct, SIGNAL(triggered()), this, SLOT(selectApp()));

    // Load default applications for current mime
    QString mime = mimeUtils->getMimeType(curIndex.filePath());
    QStringList appNames = mimeUtils->getDefault(mime);

    // Create actions for opening
    QList<QAction*> defaultApps;
    foreach (QString appName, appNames) {

      // Skip empty app name
      if (appName.isEmpty()) {
        continue;
      }

      // Load desktop file for application
      DesktopFile df = DesktopFile("/usr/share/applications/" + appName);

      // Create action
      QAction* action = new QAction(df.getName(), openMenu);
      action->setData(df.getExec());
      action->setIcon(FileUtils::searchAppIcon(df));
      defaultApps.append(action);

      // TODO: icon and connect
      connect(action, SIGNAL(triggered()), SLOT(openInApp()));

      // Add action to menu
      openMenu->addAction(action);
    }

    // Add open action to menu
    if (!defaultApps.isEmpty()) {
      openMenu->addSeparator();
    }
    openMenu->addAction(selectAppAct);
    return openMenu;
}

QMenu* corefm::globalmenu(){

    QMenu *popup = new QMenu(this);
//    QMenu *view = new QMenu(tr("View"));
    QMenu *subnew = new QMenu(tr("New.."));
    QMenu *innew = new QMenu(tr("Open in.."));
    QMenu *with = new QMenu(tr("Open with.."));
    QMenu *arrageItems = new QMenu(tr("Arrage Items"));

    QFileInfo info(selcitempath);
    QString littleinfo = info.suffix();

    sortNameAct = new QAction(tr("Name"), this);
    sortNameAct->setCheckable(true);

    sortDateAct = new QAction(tr("Date"), this);
    sortDateAct->setCheckable(true);

    sortSizeAct = new QAction(tr("Size"), this);
    sortSizeAct->setCheckable(true);

    sortByActGrp = new QActionGroup(this);
    sortByActGrp->addAction(sortNameAct);
    sortByActGrp->addAction(sortDateAct);
    sortByActGrp->addAction(sortSizeAct);
    connect(sortByActGrp, SIGNAL(triggered(QAction*)), SLOT(toggleSortBy(QAction*)));

    sortAscAct = new QAction(tr("Ascending"), this);
    sortAscAct->setCheckable(true);
    connect(sortAscAct, SIGNAL(triggered()), this, SLOT(toggleSortOrder()));

//    view->addAction(ui->actionIconView);
//    view->addAction(ui->actionDetailView);
//    if(ui->viewlist->hasFocus()){
//        view->addSeparator();
//        view->addAction(ui->actionShowThumbnails);
//    }

    innew->addAction(ui->actionNewTab);
    innew->addAction(ui->actionCoreFM);
    innew->addAction(ui->actionCoreBox);

    subnew->addAction(ui->actionNewFolder);
    subnew->addAction(ui->actionNewTextFile);

    with->addAction(ui->actionCoreImage);
    with->addAction(ui->actionCorePaint);
    with->addAction(ui->actionCorePad);
    with->addAction(ui->actionCorePlayer);

    arrageItems->addAction(sortNameAct);
    arrageItems->addAction(sortSizeAct);
    arrageItems->addAction(sortDateAct);
    arrageItems->addSeparator();
    arrageItems->addAction(sortAscAct);

    //Detect whether this is the Trash folder because menus are different
    if (ui->pathEdit->currentText() == QDir::homePath() + "/.local/share/Trash/files") { //This is the Trash folder
        popup->addSection("Nothing");
        return popup;
    }

    QModelIndexList items;
    if(listSelectionModel->selectedRows(0).count()) items = listSelectionModel->selectedRows(0);
    else items = listSelectionModel->selectedIndexes();
    int sec = items.count();

    if (sec > 1) {
         popup->addSeparator();
         popup->addAction(ui->actionCut);
         popup->addAction(ui->actionCopy);
         popup->addSeparator();
         popup->addAction(ui->actionTrash_it);
         popup->addSeparator();

    }

    else  if(sec == 1){
      if(info.isFile()){ //file
          popup->addAction(ui->actionOpen);
          popup->addMenu(with);
          popup->addMenu(createOpenWithMenu());
          popup->addSeparator();
          popup->addAction(ui->actionCut);
          popup->addAction(ui->actionCopy);
          popup->addSeparator();
          if (info.isExecutable()) {
              popup->addAction(ui->actionRunFile);
          }
        }
      if(info.isDir()){ //folder
          popup->addMenu(innew);
          popup->addSeparator();
          popup->addAction(ui->actionCut);
          popup->addAction(ui->actionCopy);
          popup->addAction(ui->actionPaste);
          popup->addSeparator();

        }
      popup->addSeparator();
      popup->addAction(ui->actionRename);
      popup->addSeparator();
      popup->addAction(ui->actionTrash_it);
      popup->addAction(ui->actionCreate_Archive);
      if(littleinfo == "zip"){
          popup->addAction(ui->actionExtract_Here);
      }
      popup->addSeparator();
      popup->addAction(ui->actionProperties);
    }
    else{ //Whitespace
        on_actionRefresh_triggered();
        popup->addAction(ui->actionSelectAll);
        popup->addSeparator();
//        popup->addMenu(view);
//        popup->addMenu(arrageItems);
        popup->addAction(ui->actionRefresh);
        popup->addSeparator();
        if (QApplication::clipboard()->mimeData()->hasUrls()) {
            popup->addAction(ui->actionPaste);
        }
        popup->addSeparator();
        popup->addMenu(subnew);
        popup->addAction(ui->actionTerminal);

        popup->addSeparator();
        popup->addAction(ui->actionProperties);
    }
    return popup;
}

/**
 * @brief Selects application for opening file
 */
void corefm::selectApp() {

  // Select application in the dialog
  ApplicationDialog *dialog = new ApplicationDialog(this);
  if (dialog->exec()) {
    if (dialog->getCurrentLauncher().compare("") != 0) {
      QString appName = dialog->getCurrentLauncher() + ".desktop";
      DesktopFile df = DesktopFile("/usr/share/applications/" + appName);
      mimeUtils->openInApp(df.getExec(), curIndex, this);
    }
  }
}

/**
 * @brief Opens file in application
 */
void corefm::openInApp() {
  QAction* action = dynamic_cast<QAction*>(sender());
  if (action) {
    mimeUtils->openInApp(action->data().toString(), curIndex, this);
  }
}

void corefm::actionMapper(QString cmd)
{
    QModelIndexList selList;
    QStringList temp;

    if(focusWidget() == ui->viewlist || focusWidget() == ui->viewtree)
    {
        QFileInfo file = modelList->fileInfo(modelView->mapToSource(listSelectionModel->currentIndex()));

        if(file.isDir())
            cmd.replace("%n",file.fileName().replace(" ","\\"));
        else
            cmd.replace("%n",file.baseName().replace(" ","\\"));

        if(listSelectionModel->selectedRows(0).count()) selList = listSelectionModel->selectedRows(0);
        else selList = listSelectionModel->selectedIndexes();
    }
    else
        selList << modelView->mapFromSource(modelList->index(curIndex.filePath()));


    cmd.replace("~",QDir::homePath());


    //process any input tokens
    int pos = 0;
    while(pos >= 0)
    {
        pos = cmd.indexOf("%i",pos);
        if(pos != -1)
        {
            pos += 2;
            QString var = cmd.mid(pos,cmd.indexOf(" ",pos) - pos);
            QString input = QInputDialog::getText(this,tr("Input"), var, QLineEdit::Normal);
            if(input.isNull()) return;              //cancelled
            else cmd.replace("%i" + var,input);
        }
    }


    foreach(QModelIndex index,selList)
        temp.append(modelList->fileName(modelView->mapToSource(index)).replace(" ","\\"));

    cmd.replace("%f",temp.join(" "));

    temp.clear();

    foreach(QModelIndex index,selList)
        temp.append(modelList->filePath(modelView->mapToSource(index)).replace(" ","\\"));

    cmd.replace("%F",temp.join(" "));

//    customActManager->execAction(cmd, ui->pathEdit->itemText(0));
}


void corefm::clearCutItems()
{
    //this refreshes existing items, sizes etc but doesn't re-sort
    modelList->clearCutItems();
    modelList->update();

    QModelIndex baseIndex = modelView->mapFromSource(modelList->index(ui->pathEdit->currentText()));

    if(currentView == 2) ui->viewtree->setRootIndex(baseIndex);
    else ui->viewlist->setRootIndex(baseIndex);
    QTimer::singleShot(50,this,SLOT(dirLoaded()));
    return;
}

bool mainTreeFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex index0 = sourceModel()->index(sourceRow, 0, sourceParent);
    myModel* fileModel = qobject_cast<myModel*>(sourceModel());

    if(fileModel->isDir(index0))
        if(this->filterRegExp().isEmpty() || fileModel->fileInfo(index0).isHidden() == 0) return true;

    return false;
}

bool viewsSortProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    if(this->filterRegExp().isEmpty()) return true;

    QModelIndex index0 = sourceModel()->index(sourceRow, 0, sourceParent);
    myModel* fileModel = qobject_cast<myModel*>(sourceModel());

    if(fileModel->fileInfo(index0).isHidden()) return false;
    else return true;
}

bool viewsSortProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    myModel* fsModel = dynamic_cast<myModel*>(sourceModel());

    if((fsModel->isDir(left) && !fsModel->isDir(right)))
        return sortOrder() == Qt::AscendingOrder;
    else if(!fsModel->isDir(left) && fsModel->isDir(right))
        return sortOrder() == Qt::DescendingOrder;

    if(left.column() == 1)          //size
    {
        if(fsModel->size(left) > fsModel->size(right)) return true;
        else return false;
    }
    else
    if(left.column() == 3)          //date
    {
        if(fsModel->fileInfo(left).lastModified() > fsModel->fileInfo(right).lastModified()) return true;
        else return false;
    }

    return QSortFilterProxyModel::lessThan(left,right);
}

QStringList myCompleter::splitPath(const QString& path) const
{
    QStringList parts = path.split("/");
    parts[0] = "/";

    return parts;
}

QString myCompleter::pathFromIndex(const QModelIndex& index) const
{
    if(!index.isValid()) return "";

    QModelIndex idx = index;
    QStringList list;
    do
    {
        QString t = model()->data(idx, Qt::EditRole).toString();
        list.prepend(t);
        QModelIndex parent = idx.parent();
        idx = parent.sibling(parent.row(), index.column());
    }
    while (idx.isValid());

    list[0].clear() ; // the join below will provide the separator

    return list.join("/");
}

void corefm::focusAction()
{
    QAction *which = qobject_cast<QAction*>(sender());
    if(which)
    {
        if(which->text().contains("address")) ui->pathEdit->setFocus(Qt::TabFocusReason);
        else if(which->text().contains("tree")) tree->setFocus(Qt::TabFocusReason);
        else if(currentView == 2) ui->viewtree->setFocus(Qt::TabFocusReason);
        else ui->viewlist->setFocus(Qt::TabFocusReason);
    }
    else
    {
        QApplication::clipboard()->blockSignals(0);
        ui->pathEdit->setCompleter(customComplete);
    }
}

void corefm::addressChanged(int old, int now)
{
    Q_UNUSED(old);
    if(!ui->pathEdit->hasFocus()) return;
    QString temp = ui->pathEdit->currentText();

    if(temp.contains("/."))
        if(!ui->showHidden->isChecked())
        {
            ui->showHidden->setChecked(1);
        }

    if(temp.right(1) == "/")
    {
        modelList->index(temp);     //make sure model has walked this folder
        modelTree->invalidate();
    }

    if(temp.length() == now) return;
    int pos = temp.indexOf("/",now);

    ui->pathEdit->lineEdit()->blockSignals(1);

    if(QApplication::keyboardModifiers() == Qt::ControlModifier)
    {
        tree->setCurrentIndex(modelTree->mapFromSource(modelList->index(temp.left(pos))));
    }
    else
    if(QApplication::mouseButtons() == Qt::MidButton)
    {
        QApplication::clipboard()->blockSignals(1);
        QApplication::clipboard()->clear(QClipboard::Selection);        //don't paste stuff

        ui->pathEdit->setCompleter(0);
        tree->setCurrentIndex(modelTree->mapFromSource(modelList->index(temp.left(pos))));
    }
    else
    if(!ui->pathEdit->lineEdit()->hasSelectedText())
    {
        ui->pathEdit->completer()->setCompletionPrefix(temp.left(pos) + "/");
        ui->pathEdit->completer()->complete();
    }

    ui->pathEdit->lineEdit()->blockSignals(0);
}

void corefm::zoomInAction()
{
    int zoomLevel;

    if(focusWidget() == tree)
    {
        (zoomTree == 64) ? zoomTree=64 : zoomTree+= 8;
        tree->setIconSize(QSize(zoomTree,zoomTree));
        zoomLevel = zoomTree;
    }
    else
    {
        if(ui->view->currentIndex() == 0)
        {
            if(ui->icon->isChecked())
            {
                (zoom == 128) ? zoom=128 : zoom+= 8;
                zoomLevel = zoom;
            }
            else
            {
                (zoomList == 128) ? zoomList=128 : zoomList+= 8;
                zoomLevel = zoomList;
            }
            on_icon_clicked(true);
        }
        else
        {
            (zoomDetail == 64) ? zoomDetail=64 : zoomDetail+= 8;
            ui->viewtree->setIconSize(QSize(zoomDetail,zoomDetail));
            zoomLevel = zoomDetail;
        }
    }

    messageEngine(QString(tr("Zoom: %1")).arg(zoomLevel), "Info");
}

void corefm::zoomOutAction()
{
    int zoomLevel;

    if(focusWidget() == tree)
    {
        (zoomTree == 16) ? zoomTree=16 : zoomTree-= 8;
        tree->setIconSize(QSize(zoomTree,zoomTree));
        zoomLevel = zoomTree;
    }
    else
    {
        if(ui->view->currentIndex() == 0)
        {
            if(ui->icon->isChecked())
            {
                (zoom == 16) ? zoom=16 : zoom-= 8;
                zoomLevel = zoom;
            }
            else
            {
                (zoomList == 16) ? zoomList=16 : zoomList-= 8;
                zoomLevel = zoomList;
            }
            on_icon_clicked(true);
        }
        else
        {
            (zoomDetail == 16) ? zoomDetail=16 : zoomDetail-= 8;
            ui->viewtree->setIconSize(QSize(zoomDetail,zoomDetail));
            zoomLevel = zoomDetail;
        }
    }

    messageEngine(QString(tr("Zoom: %1")).arg(zoomLevel), "Info");
}

void corefm::on_actionRename_triggered()
{
    if (focusWidget() == tree) {
      tree->edit(treeSelectionModel->currentIndex());
    } else if(focusWidget() == ui->viewlist) {
      ui->viewlist->edit(listSelectionModel->currentIndex());
    } else if(focusWidget() == ui->viewtree) {
      ui->viewtree->edit(listSelectionModel->currentIndex());
    }
}

void corefm::on_actionOpen_triggered()
{
    //openFile
    QModelIndexList items;
    if (listSelectionModel->selectedRows(0).count()) {
      items = listSelectionModel->selectedRows(0);
    } else {
      items = listSelectionModel->selectedIndexes();
    }

    // Executes each file of selection
    foreach (QModelIndex index, items) {
      executeFile(index, 0);
    }

//    //openfolder
//    QModelIndex i = listSelectionModel->currentIndex();
//    tree->setCurrentIndex(modelTree->mapFromSource(i));
}

void corefm::on_actionDelete_triggered()
{
    // Temporary selection info
    QModelIndexList selList;
    bool yesToAll = false;

    // Retrieves selection
    if (focusWidget() == tree) {
      selList << modelList->index(ui->pathEdit->itemText(0));
    } else {
      QModelIndexList proxyList;
      if (listSelectionModel->selectedRows(0).count()) {
        proxyList = listSelectionModel->selectedRows(0);
      } else {
        proxyList = listSelectionModel->selectedIndexes();
      }
      foreach (QModelIndex proxyItem, proxyList) {
        selList.append(modelView->mapToSource(proxyItem));
      }
    }

    bool ok = false;
    bool confirm;

    //PROBLEM
    //MESSAGE CONFIRMATION NOT NEEDED
    // Display confirmation message box
    if (sett->value("confirmDelete").isNull()) {
      QString title = tr("Delete confirmation");
      QString msg = tr("Do you want to confirm all delete operations?");
      QMessageBox::StandardButtons btns = QMessageBox::Yes | QMessageBox::No;
      if (QMessageBox::question(this, title, msg, btns) == QMessageBox::Yes) {
        confirm = 1;
      } else {
        confirm = 0;
      }
      sett->setValue("confirmDelete",confirm);
    } else {
      confirm = sett->value("confirmDelete").toBool();
    }

    // Delete selected file(s)
    for (int i = 0; i < selList.count(); ++i) {
      QFileInfo file(modelList->filePath(selList.at(i)));
      if (file.isWritable()) {
        if (file.isSymLink()) {
          ok = QFile::remove(file.filePath());
        } else {
          if (yesToAll == false) {
            if (confirm) {
              QString title = tr("Careful");
              QString msg = tr("Are you sure you want to delete <p><b>\"") + file.filePath() + "</b>?";
              int ret = QMessageBox::information(this, title, msg,
                  QMessageBox::Yes | QMessageBox::No | QMessageBox::YesToAll);
              if (ret == QMessageBox::YesToAll) yesToAll = true;
              if (ret == QMessageBox::No) return;
            }
          }
          ok = modelList->remove(selList.at(i));
        }
      } else if (file.isSymLink()) {
        ok = QFile::remove(file.filePath());
      }
    }

    // Display error message if deletion failed
    if(!ok) {
      QString msg = tr("Could not delete some items...do you have the right "
                       "permissions?");
      messageEngine(msg, "Warning");
    }

    return;
}

void corefm::on_actionBack_triggered()
{
    // If there is only one item in path edit, we cannot go back
    if (ui->pathEdit->count() == 1) return;

    // Retrieve current index
    QString current = ui->pathEdit->currentText();
    if (current.contains(ui->pathEdit->itemText(1))) {
      backIndex = modelList->index(current);
    }

    // Remove history
    do {
      ui->pathEdit->removeItem(0);
      if (tabs->count()) tabs->remHistory();
    } while (!QFileInfo(ui->pathEdit->itemText(0)).exists()
             || ui->pathEdit->itemText(0) == current);

    // Sets new dir index
    QModelIndex i = modelList->index(ui->pathEdit->itemText(0));
    tree->setCurrentIndex(modelTree->mapFromSource(i));
}

void corefm::on_actionUp_triggered()
{
    tree->setCurrentIndex(tree->currentIndex().parent());
}

void corefm::on_actionCut_triggered()
{
    // Temporary selection files
    QModelIndexList selList;
    QStringList fileList;

    // Selection
    if (focusWidget() == tree) {
      selList << modelView->mapFromSource(modelList->index(ui->pathEdit->itemText(0)));
    } else if (listSelectionModel->selectedRows(0).count()) {
      selList = listSelectionModel->selectedRows(0);
    } else {
      selList = listSelectionModel->selectedIndexes();
    }

    // Retrieve selected indices
    foreach (QModelIndex item, selList) {
      fileList.append(modelList->filePath(modelView->mapToSource(item)));
    }

    clearCutItems();
    modelList->addCutItems(fileList);

    // Save a temp file to allow pasting in a different instance
    QFile tempFile(QDir::tempPath() + "/corefm.temp");
    tempFile.open(QIODevice::WriteOnly);
    QDataStream out(&tempFile);
    out << fileList;
    tempFile.close();

    QApplication::clipboard()->setMimeData(modelView->mimeData(selList));

    modelTree->invalidate();
    listSelectionModel->clear();
    ui->paste->setVisible(true);
    messageEngine("File moves successfully.", "Info");
}

void corefm::on_actionCopy_triggered()
{
    // Selection
    QModelIndexList selList;
    if (listSelectionModel->selectedRows(0).count()) {
      selList = listSelectionModel->selectedRows(0);
    } else {
      selList = listSelectionModel->selectedIndexes();
    }

    if (selList.count() == 0) {
      if (focusWidget() == tree) {
        QModelIndex i = modelList->index(ui->pathEdit->itemText(0));
        selList << modelView->mapFromSource(i);
      } else {
        return;
      }
    }

    clearCutItems();

    QStringList text;
    foreach (QModelIndex item,selList) {
      text.append(modelList->filePath(modelView->mapToSource(item)));
    }

    QApplication::clipboard()->setText(text.join("\n"), QClipboard::Selection);
    QApplication::clipboard()->setMimeData(modelView->mimeData(selList));

    ui->actionCut->setData(0);
    ui->paste->setVisible(true);
}

void corefm::on_actionPaste_triggered()
{
    QString newPath;
    QStringList cutList;

    if (curIndex.isDir()) newPath = curIndex.filePath();
    else newPath = ui->pathEdit->itemText(0);

    // Check ui->viewlist of files that are to be cut
    QFile tempFile(QDir::tempPath() + "/corefm.temp");
    if (tempFile.exists()) {
      tempFile.open(QIODevice::ReadOnly);
      QDataStream out(&tempFile);
      out >> cutList;
      tempFile.close();
    }
    pasteLauncher(QApplication::clipboard()->mimeData(), newPath, cutList);
    ui->paste->setVisible(false);
    on_actionRefresh_triggered();
    messageEngine("Paste Completed.", "Info");
}

void corefm::on_actionProperties_triggered()
{
    QString path;

    if(selcitem==1){
       path = selcitempath;
    }else{
       path = ui->pathEdit->itemText(0);
    }
    properties = new propertiesw(path);
}

void corefm::on_actionRefresh_triggered()
{
//    QApplication::clipboard()->clear();
    listSelectionModel->clear();

    modelList->refreshItems();
    //modelTree->invalidate();
    //modelTree->sort(0, Qt::AscendingOrder);
    modelView->invalidate();
    dirLoaded();

    return;
}

void corefm::on_actionNewFolder_triggered()
{
    // Check whether current directory is writeable
    QModelIndex newDir;
    if (!QFileInfo(ui->pathEdit->itemText(0)).isWritable()) {
      messageEngine("Read only...cannot create folder", "Warning");
      return;
    }

    // Create new directory
    QModelIndex i = modelList->index(ui->pathEdit->itemText(0));
    newDir = modelView->mapFromSource(modelList->insertFolder(i));
    listSelectionModel->setCurrentIndex(newDir,QItemSelectionModel::ClearAndSelect);

    // Editation of name of new directory
    if (ui->view->currentIndex() == 0) ui->viewlist->edit(newDir);
    else ui->viewtree->edit(newDir);
}

void corefm::on_actionNewTextFile_triggered()
{
    // Check whether current directory is writeable
    QModelIndex fileIndex;
    if (!QFileInfo(ui->pathEdit->itemText(0)).isWritable()) {
      messageEngine("Read only...cannot create file", "Warning");
      return;
    }

    // Create new file
    QModelIndex i = modelList->index(ui->pathEdit->itemText(0));
    fileIndex = modelView->mapFromSource(modelList->insertFile(i));
    listSelectionModel->setCurrentIndex(fileIndex,QItemSelectionModel::ClearAndSelect);

     // Editation of name of new file
    if (ui->view->currentIndex() == 0) ui->viewlist->edit(fileIndex);
    else ui->viewtree->edit(fileIndex);
}

void corefm::on_actionNewTab_triggered()
{
    if(curIndex.isDir())
        addTab(curIndex.filePath());
}

void corefm::on_SHome_clicked()
{
    QModelIndex i = modelTree->mapFromSource(modelList->index(QDir::homePath()));
    tree->setCurrentIndex(i);
}

void corefm::on_actionTerminal_triggered()
{
    QString term = sm.getTerminal();//sett->value("Terminal").toString();//here we collect the selected terminal name from settings.

    QStringList args(term.split(" "));
    QString name = args.at(0);
    args.removeAt(0);
    QProcess::startDetached(name, args, ui->pathEdit->currentText());
    QString mess = term + " opening " ;
    messageEngine(mess, "Info");
}

void corefm::on_actionShowThumbnails_triggered(){

    if (currentView != 2) on_icon_clicked(true);
    else on_detaile_clicked(true);
    on_actionRefresh_triggered();
}

void corefm::on_actionRunFile_triggered(){

    executeFile(listSelectionModel->currentIndex(), 1);
}

void corefm::setSortColumn(QAction *columnAct) {

  // Set root index
  if (ui->viewlist->rootIndex() != modelList->index(ui->pathEdit->currentText())) {
    QModelIndex i = modelList->index(ui->pathEdit->currentText());
    ui->viewlist->setRootIndex(modelView->mapFromSource(i));
  }

  columnAct->setChecked(true);

  if (columnAct == sortNameAct) {
    currentSortColumn =  0;
  } else if (columnAct == sortDateAct) {
    currentSortColumn =  3;
  } else if (columnAct == sortSizeAct) {
    currentSortColumn = 1;
  }
  sett->setValue("Sort-Column", currentSortColumn);
}
//---------------------------------------------------------------------------

/**
 * @brief Sets sort column
 * @param action
 */
void corefm::toggleSortBy(QAction *action) {
  setSortColumn(action);
  modelView->sort(currentSortColumn, currentSortOrder);
}
//---------------------------------------------------------------------------

/**
 * @brief Sets sort order
 * @param order
 */
void corefm::setSortOrder(Qt::SortOrder order) {

  // Set root index
  if (ui->viewlist->rootIndex() != modelList->index(ui->pathEdit->currentText())) {
    QModelIndex i = modelList->index(ui->pathEdit->currentText());
    ui->viewlist->setRootIndex(modelView->mapFromSource(i));
  }

  // Change sort order
  currentSortOrder = order;
  sortAscAct->setChecked(!((bool) currentSortOrder));
  sett->setValue("Sort-Order", currentSortOrder);
}


void corefm::on_actionAscending_triggered()
{
    setSortOrder(currentSortOrder == Qt::AscendingOrder ? Qt::DescendingOrder
                                                        : Qt::AscendingOrder);
    modelView->sort(currentSortColumn, currentSortOrder);
}

/**
 * @brief Executes a file
 * @param index
 * @param run
 */
void corefm::executeFile(QModelIndex index, bool run) {

  // Index of file
  QModelIndex srcIndex = modelView->mapToSource(index);

  // Run or open
  if (run) {
    QProcess *myProcess = new QProcess(this);
    myProcess->startDetached(modelList->filePath(srcIndex));
  } else {
    mimeUtils->openInApp(modelList->fileInfo(srcIndex), this);
  }
}

void corefm::goTo(QString path) {
    if (!path.isEmpty()){
        QModelIndex i = modelTree->mapFromSource(modelList->index(path));
        tree->setCurrentIndex(i);
        on_actionRefresh_triggered();
    }
    else  {
        messageEngine("Path not exists", "Warning");
    }
}

void corefm::on_SDesktop_clicked()
{
    QModelIndex i = modelTree->mapFromSource(modelList->index(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)));
    tree->setCurrentIndex(i);
}

void corefm::on_SDownloads_clicked()
{
    QModelIndex i = modelTree->mapFromSource(modelList->index(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation)));
    tree->setCurrentIndex(i);
    on_actionRefresh_triggered();
}

void corefm::on_viewlist_customContextMenuRequested(const QPoint &pos)
{
    globalmenu()->exec(ui->viewlist->mapToGlobal(pos));
}

void corefm::on_viewtree_customContextMenuRequested(const QPoint &pos)
{
    globalmenu()->exec(ui->viewtree->mapToGlobal(pos));
}

void corefm::on_actionSelectAll_triggered()
{
    if(ui->viewlist->hasFocus()){
        ui->viewlist->selectAll();
    }
    else{
        ui->viewtree->selectAll();
    }
}

void corefm::on_Tools_clicked(bool checked)
{
    if(checked){
        ui->tools->show();
    }
    else{
        ui->tools->hide();
    }
}

void corefm::on_actionCorePlayer_triggered()
{
    CoreBox *cBox = qobject_cast<CoreBox*>(qApp->activeWindow());
    cBox->tabEngine(4, selcitempath);
}

void corefm::on_actionCorePad_triggered()
{
    CoreBox *cBox = qobject_cast<CoreBox*>(qApp->activeWindow());
    cBox->tabEngine(2, selcitempath);
}

void corefm::on_actionCoreFM_triggered()
{
    CoreBox *cBox = qobject_cast<CoreBox*>(qApp->activeWindow());
    cBox->tabEngine(0, QFileInfo(selcitempath).path());
}

void corefm::on_actionCoreImage_triggered()
{
    CoreBox *cBox = qobject_cast<CoreBox*>(qApp->activeWindow());
    cBox->tabEngine(1, selcitempath);
}

void corefm::on_actionCorePaint_triggered()
{
    CoreBox *cBox = qobject_cast<CoreBox*>(qApp->activeWindow());
    cBox->tabEngine(3, selcitempath);
}

void corefm::on_actionCoreBox_triggered()
{
    CoreBox *cBox = new CoreBox();
    cBox->show();
    QTabWidget *cTab = cBox->findChild<QTabWidget*>("windows");
    int n = cTab->count();
    corefm *cfm = new corefm();
    QString go = selcitempath;

    cfm->goTo(go);
    cTab->insertTab(n, cfm,QIcon(":/icons/CoreFM.svg"),"CoreFM");
}

void corefm::on_icon_clicked(bool checked)
{
    // Set root index
    if (ui->viewlist->rootIndex() != modelList->index(ui->pathEdit->currentText())) {
      QModelIndex i = modelList->index(ui->pathEdit->currentText());
      ui->viewlist->setRootIndex(modelView->mapFromSource(i));
    }

    if (checked) {
        ui->detaile->setChecked(false);

        ui->view->setCurrentIndex(0);
        ui->viewlist->setMouseTracking(true);
        ui->viewtree->setMouseTracking(false);

        currentView = 1;
        ui->viewlist->setViewMode(QListView::IconMode);
        ui->viewlist->setGridSize(QSize(zoom + 32, zoom + 32));
        ui->viewlist->setIconSize(QSize(zoom, zoom));
        ui->viewlist->setFlow(QListView::LeftToRight);

        modelList->setMode(ui->showthumb->isChecked());

        if (tabs->count()) tabs->setType(1);
    } else {
        ui->view->setCurrentIndex(1);
        ui->viewlist->setMouseTracking(false);
        ui->viewtree->setMouseTracking(true);
        ui->detaile->setChecked(true);
    }

    ui->viewlist->setDragDropMode(QAbstractItemView::DragDrop);
    ui->viewlist->setDefaultDropAction(Qt::MoveAction);
    on_actionRefresh_triggered();
}

void corefm::on_detaile_clicked(bool checked)
{
    if (checked) {
        ui->icon->setChecked(false);

        ui->view->setCurrentIndex(1);
        ui->viewtree->setMouseTracking(true);
        ui->viewlist->setMouseTracking(false);

        currentView = 2;
        QModelIndex i = modelList->index(ui->pathEdit->currentText());
        if (ui->viewtree->rootIndex() != i) {
          ui->viewtree->setRootIndex(modelView->mapFromSource(i));
        }

        modelList->setMode(ui->showthumb->isChecked());
        if (tabs->count()) {
          tabs->setType(2);
        }
    } else {
        ui->view->setCurrentIndex(0);
        ui->viewtree->setMouseTracking(false);
        ui->viewlist->setMouseTracking(true);
        ui->icon->setChecked(true);
    }

    on_actionRefresh_triggered();
}

void corefm::on_actionTrash_it_triggered()
{
    if (!selcitem == 0) {
        moveToTrash(selcitempath);
        on_actionRefresh_triggered();
    }
}

void corefm::on_showHidden_clicked(bool checked)
{
    if(!checked){
        if (curIndex.isHidden()) {
          listSelectionModel->clear();
        }
        modelView->setFilterRegExp("no");
        modelTree->setFilterRegExp("no");
    }
    else if (checked){
        modelView->setFilterRegExp("");
        modelTree->setFilterRegExp("");
    }
    modelView->invalidate();
    dirLoaded();
}

void corefm::on_showthumb_clicked(bool checked)
{
    Q_UNUSED(checked);
    if (currentView != 2) on_icon_clicked(true);
    else on_detaile_clicked(true);
    on_actionRefresh_triggered();
}

void corefm::on_SBookMarkIt_clicked()
{
    bookmarks bookMarks;
    QString path;
    QString Icon = ":/icons/CoreFM.svg";

    if (selcitem == 0) {
        path = ui->pathEdit->itemText(0);
        bookMarks.callBookMarkDialog(this,path, Icon);
    }
    if (selcitem == 1) {
        path =  selcitempath;
        bookMarks.callBookMarkDialog(this,path, Icon);
    }
}

void corefm::on_searchHere_clicked()
{
    QString path = ui->pathEdit->itemText(0);

    CoreBox *cBox = qobject_cast<CoreBox*>(qApp->activeWindow());
    cBox->tabEngine(11, path);
}

void corefm::on_actionExtract_Here_triggered()// engrampa -h nj.zip
{
    QProcess p1;
    QString commad = "engrampa -h \"" + selcitempath + "\"";
    p1.start(commad.toLatin1());
    p1.waitForFinished();
}

void corefm::on_actionCreate_Archive_triggered() // engrampa ./newFM/* -a nj.zip
{
    QProcess p1;
    QString commd = "engrampa \"" + selcitempath + "\" -a \"" + selcitempath + ".zip\"";
    p1.start(commd.toLatin1());
    p1.waitForFinished();
}

void corefm::on_STrash_clicked()
{
//    QModelIndex i = modelTree->mapFromSource(modelList->index(QDir::homePath() + "/.local/share/Trash/files"));
//    tree->setCurrentIndex(i);
//    on_actionRefresh_triggered();
//    ui->emptyTrash->setVisible(1);
}

void corefm::on_emptyTrash_clicked()
{
//    on_actionSelectAll_triggered();
//    on_actionDelete_triggered();
}
