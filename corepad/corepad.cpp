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


corepad::corepad(QWidget *parent) :QWidget(parent),ui(new Ui::corepad)
{
    qDebug() << "corepad opening";
    ui->setupUi(this);
    isUpdated = false;
    isSaved = false;

    ui->text->setFont(QFont(ui->text->font().family(), ui->fontSize->currentText().toInt()));
    ui->text->lineNumberArea_()->setFont(QFont(ui->text->lineNumberArea_()->font().family(), ui->fontSize->currentText().toInt()));

    ui->workingOn->setText("untitled.txt");
    ui->searchbox->setVisible(false);

    shotcuts();
}

corepad::~corepad()
{
    qDebug()<<"corepad closing";
    delete ui;
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
    if (!filePath.isNull()) {
        workFileName = filePath;
        workingFile = new QFile(filePath);
        if (!workingFile->open(QIODevice::Text | QIODevice::ReadOnly)) return;
        else {
            QTextStream in(workingFile);
            ui->text->setPlainText(in.readAll());
            workingFile->close();
            isSaved = true;
            ui->workingOn->setText(QFileInfo(workingFile->fileName()).fileName());
        }
    }
    else {
        on_cNew_clicked();
    }
}

void corepad::quiting() {
    this->deleteLater();
}

void corepad::closeEvent(QCloseEvent *event){
    if (ui->workingOn->text().contains("*")) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::warning(this, "Warning!", "Do you want to discard the changes?", QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            saveToRecent("CorePad", workFileName);
            event->accept();
        }
        else if (reply == QMessageBox::No){
            return event->ignore();
        }
    }
    else {
        saveToRecent("CorePad", workFileName);
        event->accept();
    }
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
    isUpdated = true;
    if (!ui->workingOn->text().contains("*")) {
       ui->workingOn->setText("*" + ui->workingOn->text());
    }
}

void corepad::on_cUndo_clicked()
{
    ui->text->undo();
}

void corepad::on_cRedo_clicked()
{
    ui->text->redo();
}

void corepad::on_cCut_clicked()
{
    ui->text->cut();
}

void corepad::on_cCopy_clicked()
{
    ui->text->copy();
}

void corepad::on_cPaste_clicked()
{
    ui->text->paste();
}

void corepad::on_cSave_clicked()
{
    if (!isSaved && isUpdated) {
        workFileName = QFileDialog::getSaveFileName(this, tr("Save File"),"untitled.txt",tr("Text Files (*.*)"));
        if (workFileName.isEmpty()) {
            messageEngine("Empty File Name", "Warning");
            return;
        }
        else {
            workingFile = new QFile(workFileName);
            if (!workingFile->open(QIODevice::Truncate | QIODevice::Text | QIODevice::WriteOnly)) {
                messageEngine("Can't open file", "Warning");
                return;
            }
            else {
                QTextStream out(workingFile);
                out << ui->text->toPlainText();
                workingFile->close();
                isSaved = true;
                isUpdated = false;
                ui->workingOn->setText(QFileInfo(workingFile->fileName()).fileName());
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
            out << ui->text->toPlainText();
            workingFile->close();
            isUpdated = false;
            ui->workingOn->setText(QFileInfo(workingFile->fileName()).fileName());
            messageEngine("File Saved", "Info");
        }
    }
}

void corepad::on_cSaveAs_clicked()
{
    QString file = "untitled.txt";
    if (!workFileName.isNull()) {
        file = workFileName;
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
            out << ui->text->toPlainText();
            file.flush();
            file.close();
            ui->workingOn->setText(QFileInfo(file.fileName()).fileName());
            messageEngine("File Saved", "Info");
        }
    }
}

void corepad::on_cOpen_clicked()
{
    workFileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath() + "/Documents", tr("Text Files (*.*)"));
    if (workFileName.isEmpty()) {
        messageEngine("Empty File Name", "Warning");
        return;
    }
    else {
        workingFile = new QFile(workFileName);
        if (!workingFile->open(QIODevice::Text | QIODevice::ReadOnly)) {
            messageEngine("Can't open file", "Warning");
            return;
        }
        else {
            QTextStream in(workingFile);
            ui->text->setPlainText(in.readAll());
            workingFile->close();
            isSaved = true;
            ui->workingOn->setText(QFileInfo(workingFile->fileName()).fileName());
            messageEngine("File Opened", "Info");
        }
    }
}

void corepad::on_cNew_clicked()
{
    if (ui->workingOn->text().contains("*")) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::warning(this, "Warning!", "Do you want to discard the changes?", QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            workFileName = "";
            ui->workingOn->setText("untitled.txt");
            ui->text->setPlainText("");
        }
        else if (reply == QMessageBox::No){
            return;
        }
    }
    else {
        workFileName = "";
        ui->workingOn->setText("untitled.txt");
        ui->text->setPlainText("");
    }
}

void corepad::on_cQuit_clicked()
{
    if (ui->workingOn->text().contains("*")) {
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
    if (!workFileName.isNull()) {        
        QString myIcon = ":/icons/CorePad.svg";
        bookmarks bookMarks;
        bookMarks.callBookMarkDialog(this, workFileName, myIcon);
    }
}

void corepad::on_fontSize_currentIndexChanged(const QString &arg1)
{
    ui->text->setFont(QFont(ui->text->font().family(), arg1.toInt()));
    ui->text->lineNumberArea_()->setFont(QFont(ui->text->lineNumberArea_()->font().family(), arg1.toInt()));
}

void corepad::on_addDate_clicked()
{
    int pos = ui->text->textCursor().position();
    ui->text->setPlainText(ui->text->toPlainText().insert(ui->text->textCursor().position(), QDate::currentDate().toString("dd/MM/yyyy")));
    ui->text->textCursor().setPosition(pos + 10);

}

void corepad::on_search_clicked(bool checked)
{
    if (checked){
        ui->searchbox->setVisible(true);
    }
    else {
        ui->searchbox->setVisible(false);
    }
}

void corepad::findS(QString searchS, bool reverse, QTextDocument::FindFlags flag) {
    if (!searchS.isNull()) {
        QTextCursor cr;
        ui->text->find(searchS, flag);
        cr.movePosition(reverse?QTextCursor::End:QTextCursor::Start);
        if (!(cr.position() < 0)) {
            ui->text->setTextCursor(cr);
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
