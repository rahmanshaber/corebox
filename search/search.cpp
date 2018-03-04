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

#include <QIcon>
#include <QProcess>
#include <QTableWidgetItem>
#include <QMimeDatabase>
#include <QMimeType>
#include <QTimer>
#include <QFileInfo>
#include <QThread>
#include <QThreadPool>
#include <qtconcurrentrun.h>

#include "coreimage/coreimage.h"
#include "corepad/corepad.h"
#include "corebox.h"

using namespace QtConcurrent;

search::search(QWidget *parent) :QWidget(parent),ui(new Ui::search)
{
    qDebug() << "search opening";
    ui->setupUi(this);
    ui->results->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->folderPath->setVisible(0);
    ui->typeframe->setEnabled(false);
    ui->findCMD->setEnabled(0);
    ui->locateCMD->setEnabled(0);
    shotcuts();
}

search::~search()
{
    delete ui;
}

void search::shotcuts(){
    QShortcut* shortcut;
    shortcut = new QShortcut(QKeySequence(Qt::Key_Enter), this);
    connect(shortcut, &QShortcut::activated, this, &search::on_locateCMD_clicked);
}

void search::openEngine(QString path)
{
    CoreBox *cBox = qobject_cast<CoreBox*>(qApp->activeWindow());
    QFileInfo info(path);
    //-----------------------------CoreImage---------------start--------------------------------------
    const QByteArrayList supportedMime = QImageReader::supportedImageFormats();
    foreach (const QByteArray &mimeTypeName, supportedMime) {
        if (info.completeSuffix() == mimeTypeName) {
            cBox->tabEngine(1, info.absoluteFilePath());
            return;
        }
    }
    //-----------------------------CoreImage---------------end----------------------------------------
    if (info.isDir()) {
        cBox->tabEngine(0, info.absoluteFilePath());
    }
    else {
        //-----------------------------CorePad---------------start----------------------------------------
        QString littleinfo = info.suffix();
        if(littleinfo == "txt"){
            cBox->tabEngine(2, info.absoluteFilePath());
        }
        //-----------------------------CorePad---------------end-----------------------------------------
        else {    //send it to desktop prosess
            QProcess::startDetached("xdg-open", QStringList() << path);
        }
    }
}

/**
 * @brief Set the folder path from other application
 * @param Path of the folder
 */
void search::setPath(QString text)
{
    ui->folderPath->setText(text);
}

/**
 * @brief Call the find or loacte process and collect the output and save it.
 * @param Do you want to find the file or folder related to the given name?
 * @return Return a String which is the ouput of the process
 */
QString search::processCall(bool find)
{
    //-------------Declaration-----------------------------------
    QString folPath;    //Given Folder path
    QString type;       //Type of items
    QProcess p1;        //Process
    QString ff;         //Given fileName
    QString name;       //Process argument
    //-----------------------------------------------------------

    //-------------Assigning values------------------------------
    ff = ui->searchFF->currentText();
    type = "*" + ff + "*";    
    //if folder path not given then set it to default (Root "/")
    ui->folderPath->text().isEmpty() ? folPath = "/" : folPath = ui->folderPath->text();
    //want the find process results or not then set the argument like that
    find ? name = "find " + folPath + " -iname " + type  : ui->folderPath->text().isEmpty() ? name = "locate -i /" + type : name = "locate -i " + folPath + "/" + type;
    //-----------------------------------------------------------

    //-------------Working methods-------------------------------
    ui->searchFF->addItem(ff);                      //Add the current search text
    p1.start(name.toLatin1());                      //Start process by argument
    p1.waitForFinished();                           //Wait for collecting the output
    processOutput.clear();                          //Clear the old output result
    processOutput = p1.readAllStandardOutput();     //Assign the output results of process
    return processOutput;                           //Return the output
    //-----------------------------------------------------------
}

/**
 * @brief Add items (by type) to some string list
 * @param The ouput of the process (from locate and find)
 * @return Returns a string list which have all the files found by search and also files info
 */
QStringList search::populateItems(QString text)
{
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
        if (mType.name().contains("image", Qt::CaseInsensitive)) {
            image.append(mType.iconName() + "$$$$$" + sf.at(i) + "$$$$$" + mType.preferredSuffix());
        } else if (mType.name().contains("video", Qt::CaseInsensitive)) {
            media.append(mType.iconName() + "$$$$$" + sf.at(i) + "$$$$$" + mType.preferredSuffix());
        } else if (mType.name().contains("audio", Qt::CaseInsensitive)) {
            media.append(mType.iconName() + "$$$$$" + sf.at(i) + "$$$$$" + mType.preferredSuffix());
        } else if (mType.name().contains("inode", Qt::CaseInsensitive)) {
            folder.append(mType.iconName() + "$$$$$" + sf.at(i) + "$$$$$" + mType.preferredSuffix());
        } else {
            other.append(mType.iconName() + "$$$$$" + sf.at(i) + "$$$$$" + mType.preferredSuffix());
        }
        //Add all file info to a string list
        all.append(mType.iconName() + "$$$$$" + sf.at(i) + "$$$$$" + mType.preferredSuffix());
    }
    return all;//return the all file info
    //--------------End-----------------------------------------------------------
}

/**
 * @brief Return to the specific item type
 * @return A list by type
 */
QStringList search::populateByType() {
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
    return QStringList();
}

/**
 * @brief Arrage items from a specific string list to table
 * @param A specific list which contains file icon, file path and seffix(Seperated with '$$$$$')
 */
void search::toTable(QStringList list) {
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

        ui->itemCount->setText("All : " + QString::number(all.count()) + " ;  " +
                               "Media : " + QString::number(media.count()) + " ;  " +
                               "Image : " + QString::number(image.count()) + " ;  " +
                               "Other : " + QString::number(other.count()) + " ;  " +
                               "Folder : " + QString::number(folder.count()));
    } else {
        ui->stackedWidget->setCurrentIndex(1);
        ui->itemCount->setText("0 item(s) found");
    }
}

void search::on_findCMD_clicked()
{
    if (ui->searchFF->currentText().isEmpty()) {
        return;
    } else {
        find();
    }
}

void search::find() {
    populateItems(processCall(true));
    toTable(populateByType());
}

void search::on_locateCMD_clicked()
{
    if (ui->searchFF->currentText().isEmpty()) {
        return;
    } else {
        populateItems(processCall(false));
        toTable(populateByType());
    }
}

void search::locate() {

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
    openEngine(path);
}

void search::on_more_clicked(bool checked)
{
    if(checked){
        ui->folderPath->setVisible(1);
    }else{
        ui->folderPath->setVisible(0);
    }
}

void search::on_searchFF_currentTextChanged(const QString &arg1)
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
