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

#include "bookmarks.h"
#include "ui_bookmarks.h"


bookmarks::bookmarks(QWidget *parent) :QWidget(parent),ui(new Ui::bookmarks)
{
    ui->setupUi(this);

    ui->bookmarkEditBox->setVisible(false);
    ui->addSectionBox->setVisible(false);
    ui->deleteSection->setVisible(false);
    ui->addSection->setVisible(false);
    ui->cTools->setChecked(false);
    ui->boklist->setFocusPolicy(Qt::NoFocus);

    bk.checkBook();
    sectionRefresh();
}

bookmarks::~bookmarks()
{
    delete ui;
}

bool bookmarks::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->boklist->viewport()) {
        if (event->type() == QEvent::MouseButtonDblClick) {
            QMouseEvent *mouseEvent = (QMouseEvent*) event;
            if (mouseEvent->button() == Qt::RightButton) {
                mouseEvent->ignore();
                QMessageBox::information(this,"","RIGHT");
                return false;
            }
        }
    }
    return false;
}

void bookmarks::sectionRefresh()
{
    disconnect(ui->searchBook, SIGNAL(textChanged(QString)), 0, 0);
    ui->boklist->clearSelection();
    ui->searchBook->clear();

    int selectedIndex = ui->section->currentIndex().row();

    qDebug() << bk.getBookSections();
    ui->section->clear();
    ui->section->addItems(bk.getBookSections());

    for (int i = 0; i < ui->section->count(); ++i) {
        ui->section->item(i)->setIcon((QIcon(":/icons/bookit_w.svg")));
    }

    ui->section->setCurrentRow(selectedIndex);

    if (selectedIndex > -1) {
        on_section_itemClicked(ui->section->currentItem());
    } else if (selectedIndex == -1) {
        ui->section->setCurrentRow(0);
        on_section_itemClicked(ui->section->currentItem());
    }
    connect(ui->searchBook, SIGNAL(textChanged(QString)), this, SLOT(on_searchBook_textChanged(QString)));
}

void bookmarks::on_reload_clicked()
{
    sectionRefresh();
}

/**
 * @brief Call add bookmark dialog from any apps by giving the path to book it.
 * @brief And also the apps icon.
 * @param currentPath : Path which needs to bookmarked.
 * @param iconPath : Apps icon path from resource.
 */
void bookmarks::callBookMarkDialog(QWidget *parent, const QString &currentPath)
{
    QFileInfo info(currentPath);
    BookmarkManage bm;
    const QString str = bm.checkingBookPathEx(currentPath);
    if (str.isEmpty() || str.isNull()) {
        bookmarkDialog *bkdlg = new bookmarkDialog(parent);
        QIcon ico = geticon(currentPath);
        QPixmap pix = ico.pixmap(QSize(120, 120));
        bkdlg->setBookPath(currentPath);
        bkdlg->setBookName(info.fileName() + "");
        bkdlg->checkPath();
        bkdlg->setBookIcon(pix);

        if (bkdlg->exec() == 0) {
            if (bkdlg->accepted) {
                bk.addBookmark(bkdlg->getSectionName(), bkdlg->getBookName(), currentPath);
            } else if (!bkdlg->accepted) {
                bkdlg->close();
            }
        }
        sectionRefresh();
    } else {
        // Function from globalfunctions.cpp
        messageEngine(str, MessageType::Info);
    }
}

void bookmarks::on_selectSection_currentIndexChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    on_pathName_textChanged(ui->pathName->text());
    on_bookmarkName_textChanged(ui->bookmarkName->text());
}

void bookmarks::on_section_itemClicked(QListWidgetItem *item)
{
    if (ui->section->currentItem()->text() == "Speed Dial") {
        ui->deleteSection->setEnabled(false);
    } else {
        ui->deleteSection->setEnabled(true);
    }
    ui->selectedSection->setText(item->text());
    QStringList list = bk.getBookNames(item->text());
    int count = list.count();
    if (ui->boklist->selectedItems().count() > 0) {
        ui->boklist->item(ui->boklist->currentIndex().row(), 0)->setSelected(false);
        ui->boklist->item(ui->boklist->currentIndex().row(), 1)->setSelected(false);

    }
    ui->boklist->setColumnCount(2);
    ui->boklist->setRowCount(count);

    QStringList dateTimeList;
    for (int i = 0; i < count; i++) {
        dateTimeList.append(bk.bookingTime(ui->section->currentItem()->text(), list.at(i)));
    }

    sortDateTime(dateTimeList);
    QStringList mList;
    mList.clear();
    int reverse = count - 1;
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < count; j++) {
            QString bTime = bk.bookingTime(ui->section->currentItem()->text(), list.at(j));
            if (bTime.contains(dateTimeList.at(i))) {
                mList.insert(reverse, list.at(j));
                reverse--;
            }
        }
    }
    dateTimeList.clear();
    list.clear();

    QTableWidgetItem *items;
    for (int i = 0; i < count; i++) {
        items = new QTableWidgetItem(mList.at(i));
        items->setIcon(geticon(bk.bookmarkPath(ui->section->currentItem()->text(), mList.at(i))));

        ui->boklist->setItem(i, 0, items);
        ui->boklist->setItem(i, 1, new QTableWidgetItem(bk.bookmarkPath(ui->section->currentItem()->text(), mList.at(i))));
    }
    ui->bookmarkCount->setText(QString::number(ui->boklist->rowCount()) + " item(s)");
}

void bookmarks::on_addSection_clicked()
{
    if(ui->bookmarkEditBox->isVisible()){
        on_editCancel_clicked();
    }
    ui->addSectionBox->setVisible(true);
    ui->sectionDone->setEnabled(false);
}

void bookmarks::on_deleteSection_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::warning(this, "Delete Section", "Do you want to delete this section?", QMessageBox::No | QMessageBox::Yes);
    if (reply == QMessageBox::Yes) {
        bk.delSection(ui->section->currentItem()->text());
        ui->section->takeItem(ui->section->currentIndex().row());
        // Function from globalfunctions.cpp
        messageEngine("Section Deleted", MessageType::Info);
    }
    sectionRefresh();
}

void bookmarks::on_sectionDone_clicked()
{
    bk.addSection(ui->sectionName->text());
    ui->section->addItem(ui->sectionName->text());
    ui->section->item(ui->section->count() - 1)->setIcon(QIcon(":/icons/bookit_w.svg"));
    ui->sectionName->setText("");
    ui->sectionStatus->setText("");
    ui->addSectionBox->setVisible(false);
    // Function from globalfunctions.cpp
    messageEngine("Section Added", MessageType::Info);
    on_cTools_clicked();
}

void bookmarks::on_sectionCancel_clicked()
{
    ui->sectionName->setText("");
    ui->sectionStatus->setText("");
    ui->addSectionBox->setVisible(false);
    on_cTools_clicked();
}

void bookmarks::on_sectionName_textChanged(const QString &arg1)
{
    if(arg1.count() > 0){
        if(bk.getBookSections().contains(arg1,Qt::CaseInsensitive)){
            ui->sectionStatus->setText("Section Exists");
            ui->sectionDone->setEnabled(false);
        }
        else{
            ui->sectionStatus->setText("");
            ui->sectionDone->setEnabled(true);
        }
    }

    else if (ui->sectionName->text().isEmpty()){
        ui->sectionDone->setEnabled(false);
    }
}

void bookmarks::on_bookmarkEdit_clicked()
{
    on_cTools_clicked();
    ui->bookmarkEditBox->setVisible(true);
    ui->selectSection->clear();
    ui->selectSection->addItems(bk.getBookSections());
    ui->selectSection->setCurrentText(ui->section->currentItem()->text());
    ui->bookmarkName->setText(ui->boklist->item(ui->boklist->currentIndex().row(), 0)->text());
    ui->pathName->setText(ui->boklist->item(ui->boklist->currentIndex().row(), 1)->text());
}

void bookmarks::on_bookmarkDelete_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::warning(this, "Delete Bookmark", "Do you want to delete the bookmark?", QMessageBox::No | QMessageBox::Yes);
    if (reply == QMessageBox::Yes) {
        bk.delbookmark(ui->boklist->selectedItems().at(0)->text(), ui->section->currentItem()->text());
        int r = ui->boklist->currentItem()->row();
        ui->boklist->removeRow(r);
        // Function from globalfunctions.cpp
        messageEngine("Bookmark Deleted", MessageType::Info);
    }
    ui->bookmarkCount->setText(QString::number(ui->boklist->rowCount()) + " item(s)");
    sectionRefresh();
}

void bookmarks::on_editDone_clicked()
{
    QString bookValue = QDir(ui->pathName->text()).absolutePath();
    bk.changeAll(ui->section->currentItem()->text(), ui->boklist->item(ui->boklist->currentIndex().row(), 0)->text(),
                 ui->selectSection->currentText(), ui->bookmarkName->text(), bookValue);
    if (ui->section->currentItem()->text() == ui->selectSection->currentText()) {
        int current = ui->boklist->currentIndex().row();

        ui->boklist->item(current, 0)->setText(ui->bookmarkName->text());
        ui->boklist->item(current, 1)->setText(QDir(ui->pathName->text()).absolutePath());
    } else {
        ui->boklist->removeRow(ui->boklist->currentRow());
    }
    // Function from globalfunctions.cpp
    messageEngine("Edit Done", MessageType::Info);
    sectionRefresh();
    on_editCancel_clicked();
}

void bookmarks::on_editCancel_clicked()
{
    ui->bookmarkEditBox->setVisible(false);
    ui->selectSection->clear();
    ui->bookmarkName->clear();
    ui->statuslbl->clear();
    ui->pathName->clear();
}

void bookmarks::on_bookmarkName_textChanged(const QString &arg1)
{
    if (ui->bookmarkName->text().count() > 0) {
        QString str = bk.checkingBookName(ui->selectSection->currentText(), arg1);
        if (str.count() > 0) {
            ui->statuslbl->setText(str);
            ui->editDone->setEnabled(false);
        } else {
            ui->statuslbl->setText(str);
            if (!ui->pathName->text().isNull() && ui->pathlbl->text().count() == 0) { ui->editDone->setEnabled(true); }
            else {ui->editDone->setEnabled(false);}
        }
    } else {
        ui->editDone->setEnabled(false);
    }
}

void bookmarks::on_pathName_textChanged(const QString &arg1)
{
    QFileInfo info(arg1);
    if (info.isDir()) {
        QDir dir(arg1);
        if (ui->pathName->text().count() > 0) {
            if (!QDir(dir.absolutePath()).exists()) {
                ui->pathlbl->setText(("Invalid path."));
                ui->editDone->setEnabled(false);
            } else {
                QString str = bk.checkingBookPath(ui->selectSection->currentText(), dir.absolutePath());
                if (str.count() > 0) {
                    ui->pathlbl->setText(str);
                    ui->editDone->setEnabled(false);
                } else {
                    ui->pathlbl->setText(str);
                    if (ui->bookmarkName->text().count() != 0 && ui->statuslbl->text().count() == 0) {ui->editDone->setEnabled(true);}
                    else {ui->editDone->setEnabled(false);}
                }
            }
        } else {
            ui->editDone->setEnabled(false);
        }
    } else if (info.isFile()){
        QFile file(arg1);
        if (ui->pathName->text().count() > 0) {
            if (!QFile(file.fileName()).exists()) {
                ui->pathlbl->setText(("Invalid file."));
                ui->editDone->setEnabled(false);
            } else {
                QString str = bk.checkingBookPath(ui->selectSection->currentText(), file.fileName());
                if (str.count() > 0) {
                    ui->pathlbl->setText(str);
                    ui->editDone->setEnabled(false);
                } else {
                    ui->pathlbl->setText(str);
                    if (ui->bookmarkName->text().count() != 0 && ui->statuslbl->text().count() == 0) {ui->editDone->setEnabled(true);}
                    else {ui->editDone->setEnabled(false);}
                }
            }
        } else {
            ui->editDone->setEnabled(false);
        }
    }
}

void bookmarks::on_boklist_itemDoubleClicked(QTableWidgetItem *item)
{
    QString s = bk.bookmarkPath(ui->section->currentItem()->text(), ui->boklist->item(item->row(),0)->text());

    // Function from globalfunctions.cpp
    openAppEngine(s);
}

void bookmarks::on_boklist_itemSelectionChanged()
{
    QItemSelectionModel *select = ui->boklist->selectionModel();
    int sec = select->selectedRows().count();

    if (sec == 1) {
        ui->bookmarkDelete->setEnabled(true);
        ui->bookmarkEdit->setEnabled(true);
    }
    else {
        ui->bookmarkDelete->setEnabled(false);
        ui->bookmarkEdit->setEnabled(false);
    }
}

void bookmarks::on_searchBook_textChanged(const QString &arg1)
{
    if (!arg1.isNull()) {
        for (int i = 0; i < ui->boklist->rowCount(); ++i) {
            if (ui->boklist->item(i, 0)->text().contains(arg1,Qt::CaseInsensitive)) {
                ui->boklist->item(i, 0)->setSelected(true);
                ui->boklist->item(i, 1)->setSelected(true);
            } else {
                ui->boklist->item(i, 0)->setSelected(false);
                ui->boklist->item(i, 1)->setSelected(false);
            }
        }
    } if(arg1.isEmpty()) {
        ui->boklist->clearSelection();
    }
}

void bookmarks::on_cTools_clicked()
{
    if(!ui->addSection->isVisible()){
        ui->addSection->setVisible(true);
        ui->deleteSection->setVisible(true);
    }
    else {
        ui->addSection->setVisible(false);
        ui->deleteSection->setVisible(false);

        if(ui->bookmarkEditBox->isVisible()|ui->addSectionBox->isVisible()){
            on_editCancel_clicked();
            on_sectionCancel_clicked();
        }
    }
}

void bookmarks::reload()
{
    sectionRefresh();
}
