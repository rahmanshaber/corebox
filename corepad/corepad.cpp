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

#include "corepad.h"
#include "ui_corepad.h"

#include <QDate>
#include <QDateTime>
#include <QMessageBox>


corepad::corepad(QWidget *parent) : QWidget(parent), ui(new Ui::corepad)
{
    qDebug() << "corepad opening";
    ui->setupUi(this);

    ui->searchbox->setVisible(false);

    shotcuts();
}

corepad::~corepad()
{
    qDebug()<<"corepad closing";
    delete ui;
}

bool corepad::initializeNewTab(QString filePath) {
    if (ui->notes->count() < 10) {
        isUpdated = false;
        isSaved = false;

        QString fileName;

        int index = ui->notes->tabBar()->count();
        text = new coreedit();
        //text->setPlainText("");

        workFilePath = "";
        workingFile = new QFile();
        fileName = tr("untitled%1.txt").arg(index);
        //ui->workingOn->setText(fileName);

        if (!filePath.isEmpty() || !filePath.isNull()) {
            fileName = QFileInfo(filePath).fileName();
            workFilePath = filePath;
            workingFile = new QFile(filePath);
            if (!workingFile->open(QIODevice::Text | QIODevice::ReadOnly)) return false;
            else {
                QTextStream in(workingFile);
                text->setPlainText(in.readAll());
                workingFile->close();
                isSaved = true;
                //ui->workingOn->setText(QFileInfo(workingFile->fileName()).fileName());
            }
        }

        text->setFont(QFont(text->font().family(), ui->fontSize->currentText().toInt()));
        text->lineNumberArea_()->setFont(QFont(text->lineNumberArea_()->font().family(), ui->fontSize->currentText().toInt()));

        ui->notes->insertTab(index, text, fileName);

        ui->notes->setCurrentIndex(index);
        connect(text, SIGNAL(copyAvailable(bool)), this, SLOT(on_text_copyAvailable(bool)));
        connect(text, SIGNAL(undoAvailable(bool)), this, SLOT(on_text_undoAvailable(bool)));
        connect(text, SIGNAL(redoAvailable(bool)), this, SLOT(on_text_redoAvailable(bool)));
        connect(text, SIGNAL(textChanged()), this, SLOT(on_text_textChanged()));

        return true;
    } else {
        messageEngine("Reached page limit.", "Warning");
    }
    return false;
}

void corepad::shotcuts()
{
    QShortcut* shortcut;
    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_N), this);
    connect(shortcut, &QShortcut::activated, this, &corepad::on_cNew_clicked);
    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_O), this);
    connect(shortcut, &QShortcut::activated, this, &corepad::on_cOpen_clicked);
    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_S), this);
    connect(shortcut, &QShortcut::activated, this, &corepad::on_cSave_clicked);
    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_S), this);
    connect(shortcut, &QShortcut::activated, this, &corepad::on_cSaveAs_clicked);
    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Z), this);
    connect(shortcut, &QShortcut::activated, this, &corepad::on_cUndo_clicked);
    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Y), this);
    connect(shortcut, &QShortcut::activated, this, &corepad::on_cRedo_clicked);
    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_C), this);
    connect(shortcut, &QShortcut::activated, this, &corepad::on_cCopy_clicked);
    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_X), this);
    connect(shortcut, &QShortcut::activated, this, &corepad::on_cCut_clicked);
    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_V), this);
    connect(shortcut, &QShortcut::activated, this, &corepad::on_cPaste_clicked);
    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q), this);
    connect(shortcut, &QShortcut::activated, this, &corepad::on_cQuit_clicked);
}

void corepad::openText(QString filePath) {
    initializeNewTab(filePath);
}

void corepad::quiting() {
    this->deleteLater();
}

void corepad::closeEvent(QCloseEvent *event) {
//    event->ignore();
//    for (int i = 0; i < ui->notes->count(); i++) {
//        qDebug() << i << ui->notes->count();
//        if (ui->notes->tabText(i).contains("*")) {
//            qDebug() << i;
//            QMessageBox::StandardButton reply;
//            reply = QMessageBox::warning(this, "Warning!", "Do you want to discard the changes?", QMessageBox::Yes | QMessageBox::No);
//            if (reply == QMessageBox::Yes) {
//                saveToRecent("CorePad", workFilePath);
//                ui->notes->removeTab(i);
//            }
//            else if (reply == QMessageBox::No){
//                //return;
//            }
//        } else {
//            saveToRecent("CorePad", workFilePath);
//            ui->notes->removeTab(i);
//            qDebug() << "else" << i << ui->notes->count();
//        }
//        on_notes_tabCloseRequested(i);
//    }


//    if (ui->notes->count() == 0) {
//        qDebug() << "count" << ui->notes->count();
//        event->accept();
//    }
//    else event->ignore();

    if (closeAllTab()) {
        event->ignore();
        saveToRecent("CorePad", workFilePath);
    } else return event->ignore();
}

bool corepad::closeAllTab() {
    while (ui->notes->count() != 0) {
        if (ui->notes->tabText(ui->notes->currentIndex()).startsWith("*")) {
            //int ans = QMessageBox::warning(this, "Warning!", "Do you want to discard the changes?", QMessageBox::Yes | QMessageBox::No);
            QMessageBox::StandardButton reply;
            reply = QMessageBox::warning(this, "Warning!", "Do you want to discard the changes?", QMessageBox::Yes | QMessageBox::No);

                 if (reply == QMessageBox::No) {
                    return false;
                 } else if (reply == QMessageBox::Yes){
                     text = static_cast<coreedit*>(ui->notes->widget(ui->notes->currentIndex()));
                     text->deleteLater();
                     qDebug() << ui->notes->currentIndex();
                     ui->notes->removeTab(ui->notes->currentIndex());
                     return true;
                 }
            }


    }
    return true;
}

void corepad::on_text_copyAvailable(bool b)
{
    ui->cCopy->setEnabled(b);
    ui->cCut->setEnabled(b);
}

void corepad::on_text_undoAvailable(bool b)
{
    ui->cUndo->setEnabled(b);
}

void corepad::on_text_redoAvailable(bool b)
{
    ui->cRedo->setEnabled(b);
}

void corepad::on_text_textChanged()
{
    if (text->toPlainText().count() > 0) {
        isUpdated = true;
        if (!ui->notes->tabText(ui->notes->currentIndex()).contains("*")) {
           ui->notes->setTabText(ui->notes->currentIndex(), "*" + ui->notes->tabText(ui->notes->currentIndex()));
        }
    }
}

void corepad::on_cUndo_clicked()
{
    text->undo();
}

void corepad::on_cRedo_clicked()
{
    text->redo();
}

void corepad::on_cCut_clicked()
{
    text->cut();
}

void corepad::on_cCopy_clicked()
{
    text->copy();
}

void corepad::on_cPaste_clicked()
{
    text->paste();
}

void corepad::on_cSave_clicked()
{
    if (!isSaved && isUpdated) {
        workFilePath = QFileDialog::getSaveFileName(this, tr("Save File"),"untitled.txt",tr("Text Files (*.*)"));
        if (workFilePath.isEmpty()) {
            messageEngine("Empty File Name", "Warning");
            return;
        }
        else {
            workingFile = new QFile(workFilePath);
            if (!workingFile->open(QIODevice::Truncate | QIODevice::Text | QIODevice::WriteOnly)) {
                messageEngine("Can't open file", "Warning");
                return;
            }
            else {
                QTextStream out(workingFile);
                out << text->toPlainText();
                workingFile->close();
                isSaved = true;
                isUpdated = false;
                ui->notes->setTabText(ui->notes->currentIndex(), QFileInfo(workingFile->fileName()).fileName());
                messageEngine("File Saved", "Info");
            }
        }
    }
    else if (isSaved && isUpdated) {
        if (!workingFile->open(QIODevice::Truncate | QIODevice::Text | QIODevice::WriteOnly)) {
            messageEngine("Can't open file", "Warning");
            return;
        }
        else {
            QTextStream out(workingFile);
            out << text->toPlainText();
            workingFile->close();
            isUpdated = false;
            ui->notes->setTabText(ui->notes->currentIndex(), QFileInfo(workingFile->fileName()).fileName());
            messageEngine("File Saved", "Info");
        }
    }
}

void corepad::on_cSaveAs_clicked()
{
    QString file = "untitled.txt";
    if (!workFilePath.isNull()) {
        file = workFilePath;
    }
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File As"),
                                                      file,
                                                      tr("Text Files (*.*)"));
    if (fileName.isEmpty()) {
        messageEngine("Empty File Name", "Warning");
        return;
    }
    else {
        QFile file(fileName);
        if (!file.open(QIODevice::Truncate | QIODevice::Text | QIODevice::WriteOnly)) {
            messageEngine("Can't open file", "Warning");
            return;
        }
        else {
            QTextStream out(&file);
            out << text->toPlainText();
            file.flush();
            file.close();
            ui->notes->setTabText(ui->notes->currentIndex(), QFileInfo(file.fileName()).fileName());
            messageEngine("File Saved", "Info");
        }
    }
}

void corepad::on_cOpen_clicked()
{
    workFilePath = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath() + "/Documents", tr("Text Files (*.*)"));
    if (workFilePath.isEmpty()) {
        messageEngine("Empty File Name", "Warning");
        return;
    }
    else {
        bool open = initializeNewTab(workFilePath);
        if (open) messageEngine("File Opened", "Info");
        else messageEngine("Can't open file", "Warning");
    }
}

void corepad::on_cNew_clicked()
{
    initializeNewTab(NULL);
}

void corepad::on_cQuit_clicked()
{
    if (ui->notes->tabText(ui->notes->currentIndex()).contains("*")) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::warning(this, "Warning!", "Do you want to discard the changes?", QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            close();
        }
        else if (reply == QMessageBox::No){
            return;
        }
    }
    else {
        close();
    }
}

void corepad::on_bookMarkIt_clicked()
{
    if (!workFilePath.isNull()) {
        bookmarks bookMarks;
        bookMarks.callBookMarkDialog(this, workFilePath);
    }
}

void corepad::on_fontSize_currentIndexChanged(const QString &arg1)
{
    text->setFont(QFont(text->font().family(), arg1.toInt()));
    text->lineNumberArea_()->setFont(QFont(text->lineNumberArea_()->font().family(), arg1.toInt()));
}

void corepad::on_addDate_clicked()
{
    int pos = text->textCursor().position();
    text->setPlainText(text->toPlainText().insert(text->textCursor().position(), QDate::currentDate().toString("dd/MM/yyyy")));
    text->textCursor().setPosition(pos + 10);

}

void corepad::on_search_clicked(bool checked)
{
    if (checked) {
        ui->searchbox->setVisible(true);
    } else {
        ui->searchbox->setVisible(false);
    }
}

void corepad::findS(QString searchS, bool reverse, QTextDocument::FindFlags flag) {
    if (!searchS.isNull()) {
        QTextCursor cr;
        text->find(searchS, flag);
        cr.movePosition(reverse ? QTextCursor::End : QTextCursor::Start);
        if (!(cr.position() < 0)) {
            text->setTextCursor(cr);
        }
    }
}

void corepad::on_searchHere_textChanged(const QString &arg1)
{
    findS(arg1, false);
}

void corepad::on_nextW_clicked()
{
    findS(ui->searchHere->text(), false);
}

void corepad::on_previousW_clicked()
{
    findS(ui->searchHere->text(), true, QTextDocument::FindBackward);
}

void corepad::on_notes_currentChanged(int index)
{
    if(ui->notes->count() > 0){
        text = static_cast<coreedit*>(ui->notes->widget(index));
        /*Copy available need to fix when the current tab changed*/
        on_text_redoAvailable(text->isUndoRedoEnabled());
        on_text_undoAvailable(text->isUndoRedoEnabled());
        on_text_textChanged();
        on_searchHere_textChanged(ui->searchHere->text());
    }
}

void corepad::on_notes_tabCloseRequested(int index)
{
    if (ui->notes->tabBar()->tabText(index).contains("*")) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::warning(this, "Warning!", "Do you want to discard the changes?", QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            ui->notes->tabBar()->removeTab(index);
            goto L;
        }
        else if (reply == QMessageBox::No){
            return;
        }
    }
    else {
        L:
        text = static_cast<coreedit*>(ui->notes->widget(index));
        text->deleteLater();
        ui->notes->removeTab(index);
        return;
    }
}
