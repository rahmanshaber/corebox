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

#include "search.h"
#include "ui_search.h"


search::search(QWidget *parent) :QWidget(parent),ui(new Ui::search)
{
    ui->setupUi(this);

    startsetup();
}

search::~search()
{
    delete ui;
    delete cProcess;
    all.clear();
    media.clear();
    image.clear();
    other.clear();
    folder.clear();
}

void search::startsetup()
{
    ui->results->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->pathfream->setVisible(0);

    ui->findCMD->setEnabled(0);
    ui->locateCMD->setEnabled(0);

    ui->typeframe->setVisible(0);
    ui->cancelProc->setVisible(0);

    // Add setting for deleting the search activity.
    bool isActivityEnabled = true;

    // Search Activity file Path
    QString sActFile = QDir::homePath() + "/.config/coreBox/SearchActivity";
    QSettings searchActF(sActFile);

    int count = searchActF.childGroups().count();
    if (isActivityEnabled) {
        ui->activityList->clear();
        QStringList toplevel = searchActF.childGroups();
        foreach (QString group, toplevel) {
            QTreeWidgetItem *topTree = new QTreeWidgetItem;
            QString groupL = sentDateText(group);
            topTree->setText(0, groupL);
            searchActF.beginGroup(group);
            QStringList keys = searchActF.childKeys();
            QCollator sort;
            sort.setNumericMode(1);
            std::sort(keys.begin(), keys.end(), sort);
            foreach (QString key, keys) {
                QTreeWidgetItem *child = new QTreeWidgetItem;
                QString value = searchActF.value(key).toString();
                child->setText(0, value);
                topTree->addChild(child);
            }
            searchActF.endGroup();
            ui->activityList->insertTopLevelItem(0, topTree);
        }

        if (toplevel.count())
            ui->activityList->setExpanded(ui->activityList->model()->index(0, 0), true);
    }

    bool isSearchActivityAvailable = true;
    if (!count) isSearchActivityAvailable = 0;

    if (isSearchActivityAvailable) {
        ui->infoPage->setCurrentIndex(2);
    } else {
        ui->infoPage->setCurrentIndex(1);
        ui->typeframe->setVisible(0);
        ui->status->setText("Search for Something by file type\nEnter text you want to search.");
    }

    ui->searchFF->setFocus();

    shotcuts();

    cProcess = new QProcess(this);

    connect(cProcess, &QProcess::started, [&]() {
        ui->status->setText("Collecting Information...\nPlease wait for a moment...");

        ui->cancelProc->setVisible(1);
        ui->findCMD->setEnabled(0);
        ui->folderPath->setEnabled(0);
        ui->activityList->setEnabled(0);
        ui->locateCMD->setEnabled(0);
        ui->typePicture->setEnabled(0);
        ui->typeAll->setEnabled(0);
        ui->typeFolder->setEnabled(0);
        ui->typeMedia->setEnabled(0);
        ui->typeother->setEnabled(0);
        //ui->typeframe->setVisible(0);
        ui->more->setEnabled(0);
        ui->setfolder->setEnabled(0);
        ui->title->setEnabled(0);
    });

    connect(ui->cancelProc, &QPushButton::clicked, cProcess, &QProcess::kill);

    connect(cProcess, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
          [=](int exitCode, QProcess::ExitStatus exitStatus){
        Q_UNUSED(exitCode);
        //Q_UNUSED(exitStatus);
        if (exitStatus == QProcess::NormalExit) {
            QTextStream text(cProcess->readAllStandardOutput());
            populateItems(text.readAll());
        } else {
            ui->infoPage->setCurrentIndex(1);
            ui->status->setText("You select cancel for not to search");
        }

        ui->findCMD->setEnabled(1);
        ui->folderPath->setEnabled(1);
        ui->activityList->setEnabled(1);
        ui->locateCMD->setEnabled(1);
        ui->typePicture->setEnabled(1);
        ui->typeAll->setEnabled(1);
        ui->typeFolder->setEnabled(1);
        ui->typeMedia->setEnabled(1);
        ui->typeother->setEnabled(1);
        //ui->typeframe->setVisible(1);
        ui->more->setEnabled(1);
        ui->setfolder->setEnabled(1);
        ui->title->setEnabled(1);

        ui->cancelProc->setVisible(0);

        cProcess->close();
    });
}

/**
 * @brief Set the folder path from other application
 * @param Path of the folder
 */
void search::setPath(const QString &path)
{
    if(!path.isEmpty()){ui->folderPath->setText(path);}
    if(path.isEmpty()){ui->folderPath->setText("/");}
}

/**
 * @brief search::callProcess Call a find or locate for searching.
 * @brief Create a process based on that. And the process will collect the info.
 * @param find Use find or locate process for searching.
 */
void search::callProcess(bool find)
{
    QString programName;
    QString argument;
    QString path = ui->folderPath->text();
    QString pattern = ui->searchFF->text(); //Search Text

    if (path.isEmpty()) path = "/";
    if (!QFileInfo(path).isDir()) path = QFileInfo(path).path();
    if (!QFileInfo(path).exists()) path = "/";

    if (pattern.isEmpty()) pattern = "*";
    else pattern = "*" + pattern + "*";

    path = "\"" + path + "\"";
    if (find) {
        programName = "find";
        argument = path + " -iname \"" + pattern + "\"";
    } else {
        if (path.count() == 3) path = "";
        programName = "locate";
        argument = "-i " + path + "/\"" + pattern + "\"";
    }

    cProcess->start(QString(programName + " " + argument).toLatin1());
}

/**
 * @brief Add items (by type) to some string list
 * @param The ouput of the process (from locate and find)
 * @return Returns a string list which have all the files found by search and also files info
 */
void search::populateItems(const QString &text)
{
    QFuture<void> future = QtConcurrent::run([this, text]() {
        //----------Declaration--------------------------------------
        QMimeDatabase mime;
        QMimeType mType;
        QStringList sf;
        int c;
        //-----------End---------------------------------------------

        //-----------Calling methods and assigning values------------
        //Clearing the old items
        all.clear();
        media.clear();
        image.clear();
        other.clear();
        folder.clear();
        sf.clear();
        //------------End--------------------------------------------

        //-----------------------------------------------------------------------
        //Split the text by a single line and assign the result to a string list
        sf = text.split("\n");

        sf.removeLast();//Remove the last entry (An empty line)

        c = sf.count();//Collect the count of lines of process output

        //if (c < 1)
        //    toTable();

        for (int i = 0; i < c; ++i) {
            //Collect mime type from list (of all items)
            mType = mime.mimeTypeForFile(sf.at(i));
            //Check is it image, media (audio, video), folder if not then other
            //Append result info to a image, media, folder and other string list by icon name file whole path and the suffix
            //'$$$$$' used for setting the file info seperately
            if (mType.name().startsWith("image", Qt::CaseInsensitive)) {
                image.append(mType.iconName() + "$$$$$" + sf.at(i) + "$$$$$" + mType.preferredSuffix());
            } else if (mType.name().startsWith("video", Qt::CaseInsensitive)) {
                media.append(mType.iconName() + "$$$$$" + sf.at(i) + "$$$$$" + mType.preferredSuffix());
            } else if (mType.name().startsWith("audio", Qt::CaseInsensitive)) {
                media.append(mType.iconName() + "$$$$$" + sf.at(i) + "$$$$$" + mType.preferredSuffix());
            } else if (mType.name().startsWith("inode", Qt::CaseInsensitive)) {
                folder.append(mType.iconName() + "$$$$$" + sf.at(i) + "$$$$$" + mType.preferredSuffix());
            } else {
                other.append(mType.iconName() + "$$$$$" + sf.at(i) + "$$$$$" + mType.preferredSuffix());
            }
            //Add all file info to a string list
            all.append(mType.iconName() + "$$$$$" + sf.at(i) + "$$$$$" + mType.preferredSuffix());
        }
    });

    QFutureWatcher<void>* r = new QFutureWatcher<void>();
    r->setFuture(future);
    connect(r, &QFutureWatcher<void>::finished, [&](){
        //ui->status->setText("NO ITEM FOUND.");
        ui->infoPage->setCurrentIndex(0);
        ui->typeframe->setVisible(1);

        // Check is search activity enabled or not

        // Search Activity file Path
        QString sActFile = QDir::homePath() + "/.config/coreBox/SearchActivity";
        QSettings searchActF(sActFile);
        QDateTime currentDT = QDateTime::currentDateTime();
        QString group = currentDT.toString("dd.MM.yyyy");
        QString key = currentDT.toString("hh.mm.ss");
        searchActF.beginGroup(group);
        QString path = "/";
        if (QFileInfo(ui->folderPath->text()).isDir()) path = ui->folderPath->text();
        searchActF.setValue(key, ui->searchFF->text() + "\t\t\t" + path);
        searchActF.endGroup();
//        QFile file(sActFile);
//        if (file.open(QIODevice::Append | QIODevice::Text | QIODevice::ReadWrite)) {
//            QString path = "/";
//            if (QFileInfo(ui->folderPath->text()).isDir()) path = ui->folderPath->text();
//            file.write(QString(ui->searchFF->text() + "$$$" + path + "$$$" + QString::number(all.count()) + "\n").toLatin1());
//            file.close();
//        }
        ui->typeAll->setChecked(1);
        toTable(populateByType());
    });
}

/**
 * @brief Return to the specific item type
 * @return A list by type
 */
const QStringList& search::populateByType()
{
    if (ui->typeFolder->isChecked()) {
        return folder;
    } else if (ui->typeMedia->isChecked()) {
        return media;
    } else if (ui->typePicture->isChecked()) {
        return image;
    } else if (ui->typeother->isChecked()) {
        return other;
    } else if (ui->typeAll->isChecked()) {
        return all;
    }
    static const QStringList empty;
    return empty;
}

/**
 * @brief Arrage items from a specific string list to table
 * @param A specific list which contains file icon, file path and seffix(Seperated with '$$$$$')
 */
void search::toTable(const QStringList &list)
{
    QFuture<void> f = QtConcurrent::run([this, list](){
        QStringList temp;

        // Collect Information and add it to tablewidget
        if (list.count() > 0) {
            ui->infoPage->setCurrentIndex(0);
            ui->typeframe->setVisible(1);
            ui->results->clearContents();//Clear the rows from the table
            ui->results->setRowCount(list.count());//set row count by list item count

            for (int i = 0; i < list.count(); ++i) {
                //get one item from given list and split it by '$$$$$'
                //and assign it to another string list
                temp = list.at(i).split("$$$$$");
                //the first item at temp string list is theme icon name
                //second is file path
                //third is suffix
                ui->results->setItem(i, 0, new QTableWidgetItem(QIcon::fromTheme(temp.at(0)), QFileInfo(temp.at(1)).fileName()));
                ui->results->setItem(i, 1, new QTableWidgetItem(QFileInfo(temp.at(1)).path()));
                ui->results->setItem(i, 2, new QTableWidgetItem(temp.at(2)));
            }

        } else {
            ui->infoPage->setCurrentIndex(1);
//            ui->typeframe->setVisible(0);
            ui->status->setText("NO ITEMS FOUND...");
            ui->itemCount->setText("0 item(s) found");
        }
    });

    QFutureWatcher<void> *w = new QFutureWatcher<void>();
    w->setFuture(f);
    connect(w, &QFutureWatcher<void>::finished, [&](){
        ui->itemCount->setText("All : " + QString::number(all.count()) + " ;  " +
                               "Media : " + QString::number(media.count()) + " ;  " +
                               "Image : " + QString::number(image.count()) + " ;  " +
                               "Other : " + QString::number(other.count()) + " ;  " +
                               "Folder : " + QString::number(folder.count()));
    });
}

void search::shotcuts()
{
    connect(ui->searchFF, &QLineEdit::returnPressed, this, &search::on_findCMD_clicked);
}

void search::checkChange(QToolButton *a, QToolButton *b, QToolButton *c, QToolButton *d, QToolButton *e)
{
    if (a->isChecked()) {
        b->setChecked(false);
        c->setChecked(false);
        d->setChecked(false);
        e->setChecked(false);
    } /*else {
        b->setChecked(true);
    }*/
}

void search::on_findCMD_clicked()
{
    if (ui->searchFF->text().isEmpty()) {
        return;
    } else {
        ui->infoPage->setCurrentIndex(1);
        ui->typeframe->setVisible(0);
        ui->results->clearContents();
        ui->typeAll->setChecked(0);
        ui->typeMedia->setChecked(0);
        ui->typeFolder->setChecked(0);
        ui->typeother->setChecked(0);
        ui->typePicture->setChecked(0);
        callProcess(true);
    }
}

void search::on_locateCMD_clicked()
{
    if (ui->searchFF->text().isEmpty()) {
        return;
    } else {
        ui->infoPage->setCurrentIndex(1);
        ui->typeframe->setVisible(0);
        ui->results->clearContents();
        ui->typeAll->setChecked(0);
        ui->typeMedia->setChecked(0);
        ui->typeFolder->setChecked(0);
        ui->typeother->setChecked(0);
        ui->typePicture->setChecked(0);
        callProcess(false);
    }
}

void search::on_typeAll_clicked()
{
    checkChange(ui->typeAll, ui->typePicture, ui->typeFolder, ui->typeMedia, ui->typeother);
    toTable(populateByType());
}

void search::on_typePicture_clicked()
{
    checkChange(ui->typePicture, ui->typeAll, ui->typeFolder, ui->typeMedia, ui->typeother);
    toTable(populateByType());
}

void search::on_typeFolder_clicked()
{
    checkChange(ui->typeFolder, ui->typeAll, ui->typePicture, ui->typeMedia, ui->typeother);
    toTable(populateByType());
}

void search::on_typeMedia_clicked()
{
    checkChange(ui->typeMedia, ui->typeAll, ui->typePicture, ui->typeFolder, ui->typeother);
    toTable(populateByType());
}

void search::on_typeother_clicked()
{
    checkChange(ui->typeother, ui->typeAll, ui->typePicture, ui->typeFolder, ui->typeMedia);
    toTable(populateByType());
}

void search::on_results_itemDoubleClicked(QTableWidgetItem *item)
{
    // Function from globalfunctions.cpp
    QString path = ui->results->item(item->row(), 1)->text() + "/" + ui->results->item(item->row(), 0)->text();
    openAppEngine(path);
}

void search::on_more_clicked(bool checked)
{
    if(checked){
        ui->pathfream->setVisible(1);
        ui->folderPath->setFocus();
    }else{
        ui->pathfream->setVisible(0);
        ui->searchFF->setFocus();
    }
}

void search::on_searchFF_textChanged(const QString &arg1)
{
    if(arg1.isEmpty()){
        ui->findCMD->setEnabled(0);
        ui->locateCMD->setEnabled(0);
        ui->typeframe->setEnabled(false);
        ui->typeframe->setVisible(false);
        ui->infoPage->setCurrentIndex(2);
    }
    else{
        ui->findCMD->setEnabled(1);
        ui->locateCMD->setEnabled(1);
        ui->typeframe->setEnabled(true); ;
        ui->typeframe->setVisible(false);
    }
}

void search::on_setfolder_clicked()
{
    ui->folderPath->setText(QFileDialog::getExistingDirectory(this, "Select a folder"));
}
