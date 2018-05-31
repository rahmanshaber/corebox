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

#include "coreimage/coreimage.h"
#include "corepad/corepad.h"
#include "corebox/corebox.h"

#include <QIcon>
#include <QMimeDatabase>
#include <QMimeType>
#include <QFileInfo>
#include <QtConcurrent>

search::search(QWidget *parent) :QWidget(parent),ui(new Ui::search)
{
    qDebug() << "search opening";
    ui->setupUi(this);
    ui->searchFF->setFocus();
    ui->results->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->pathfream->setVisible(0);
    ui->typeframe->setEnabled(false);
    ui->findCMD->setEnabled(0);
    ui->locateCMD->setEnabled(0);
    shotcuts();

    cProcess = new QProcess(this);

    connect(cProcess, &QProcess::started, [&]() {
        ui->label_2->setText("Collecting Information...\nPlease wait for a moment...");
    });

    connect(cProcess, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
          [=](int exitCode, QProcess::ExitStatus exitStatus){
        Q_UNUSED(exitCode);
        Q_UNUSED(exitStatus);
        populateItems(cProcess->readAllStandardOutput());
        cProcess->close();
    });
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
void search::callProcess(bool find) {
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
        ui->label_2->setText("NO ITEM FOUND.");
        ui->stackedWidget->setCurrentIndex(0);
        toTable(populateByType());
    });
}

/**
 * @brief Return to the specific item type
 * @return A list by type
 */
const QStringList& search::populateByType() {
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
void search::toTable(const QStringList &list) {

    QFuture<void> f = QtConcurrent::run([this, list](){
        QStringList temp;

        if (list.count() > 0) {
            ui->stackedWidget->setCurrentIndex(0);
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
            ui->stackedWidget->setCurrentIndex(1);
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

void search::shotcuts(){
    QShortcut* shortcut;
    shortcut = new QShortcut(QKeySequence(Qt::Key_Enter), this);
    connect(shortcut, &QShortcut::activated, this, &search::on_locateCMD_clicked);
}

void search::checkChange(QToolButton *a, QToolButton *b, QToolButton *c, QToolButton *d, QToolButton *e) {
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
        ui->stackedWidget->setCurrentIndex(1);
        ui->results->clearContents();
        callProcess(true);
    }
}

void search::on_locateCMD_clicked()
{
    if (ui->searchFF->text().isEmpty()) {
        return;
    } else {
        ui->stackedWidget->setCurrentIndex(1);
        ui->results->clearContents();
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
        ui->typeframe->setFocusPolicy(Qt::NoFocus);
    }
    else{
        ui->findCMD->setEnabled(1);
        ui->locateCMD->setEnabled(1);
        ui->typeframe->setEnabled(true); ;
        ui->typeframe->setFocusPolicy(Qt::NoFocus);
    }
}

void search::on_setfolder_clicked()
{
    ui->folderPath->setText(QFileDialog::getExistingDirectory(this, "Select a folder"));
}
