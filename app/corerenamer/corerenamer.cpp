#include "corerenamer.h"
#include "ui_corerenamer.h"

#include "undocommands.h"

#include <QFileDialog>
#include <QListWidgetItem>
#include <QMimeDatabase>
#include <QIcon>
#include <QFileInfo>
#include <QDir>
#include <QMimeType>
#include <QDebug>
#include <QIntValidator>
#include <QCollator>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QShortcut>
#include <QUndoCommand>
#include <QUndoGroup>
#include <QUndoStack>
#include <QUndoView>

corerenamer::corerenamer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::corerenamer)
{
    ui->setupUi(this);

    ui->setupUi(this);

    uStack = new QUndoStack(this);
    ui->addTextCPos->setValidator(new QIntValidator());
    ui->removeTextCPos->setValidator(new QIntValidator());
    ui->removeTextCCount->setValidator(new QIntValidator());

    ui->bUndo->setEnabled(false);
    ui->bRedo->setEnabled(false);

    QUndoView *view = new QUndoView(uStack, this);
    view->show();
    view->resize(200, 200);
    view->move(this->pos().x() + 250, this->pos().y() + 25);

    connect(uStack, &QUndoStack::canUndoChanged, [this](bool canUndo) {
        ui->bUndo->setEnabled(canUndo);
    });
    connect(uStack, &QUndoStack::canRedoChanged, [this](bool canRedo) {
        ui->bRedo->setEnabled(canRedo);
    });

    createActions();
}

corerenamer::~corerenamer()
{
    delete ui;
}

void corerenamer::createActions()
{
    QShortcut *shortcut;

    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_N), this);
    connect(shortcut, &QShortcut::activated, this, &corerenamer::on_bAddFiles_clicked);

    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Z), this);
    connect(shortcut, &QShortcut::activated, this, &corerenamer::on_bUndo_clicked);

    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_Z), this);
    connect(shortcut, &QShortcut::activated, this, &corerenamer::on_bRedo_clicked);
}

QIcon corerenamer::fileIcon(const QString &filePath)
{
    QMimeDatabase mimes;
    QMimeType mime = mimes.mimeTypeForFile(filePath);
    return QIcon::fromTheme(mime.iconName());
}

QString corerenamer::CapitalizeEachWord(const QString &str)
{
    QStringList sep = str.split(' ');
    QString total = "";
    foreach (QString s, sep) {
        if (!s.count())
            continue;

        foreach (QChar c, s) {
            if (c.isLetter()) {
                s = s.toLower().replace(s.indexOf(c), 1, c.toTitleCase());
                break;
            }
        }
        total = total + " " + s;
    }

    return total.remove(0, 1);
}

void corerenamer::addFiles(QStringList list)
{
    ui->filesList->clear();
    uStack->clear();
    QTableWidgetItem *sItem;

    QString path = QFileInfo(list.at(0)).path();

    for (int i = 0; i < list.count(); i++)
        list.replace(i, QString(list.at(i)).remove(path));

    QCollator collator;
    collator.setNumericMode(true);
    std::sort(list.begin(), list.end(), collator);

    for (int i = 0; i < list.count(); i++)
        list.replace(i, path + list.at(i));

    ui->filesList->setColumnCount(2);
    ui->filesList->setHorizontalHeaderLabels(QStringList() << "Selected" << "Changed");
    ui->filesList->setRowCount(list.count());

    for (int i = 0; i < list.count(); ++i) {
        sItem = new QTableWidgetItem(QFileInfo(list.at(i)).completeBaseName());
        ui->filesList->setItem(i, 0, sItem);
        ui->filesList->setItem(i, 1, new QTableWidgetItem(""));
        ui->filesList->item(i, 1)->setData(Qt::UserRole, list.at(i));
        sItem->setIcon(fileIcon(list.at(i)));
        sItem->setData(Qt::UserRole, list.at(i));
    }

    ui->filesList->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

QString corerenamer::addText(tPosition tpos, int pos, const QString &newText, const QString &selcText)
{
    int initialPos = 0;
    int addPos = pos;

    qDebug() << "BEFORE " << selcText;

    if (tpos == FROMRIGHT) {
        initialPos = selcText.length();
        addPos = initialPos - pos;
    }

    qDebug() << "AFTER " << QString(selcText).insert(addPos, newText);

    return  QString(selcText).insert(addPos, newText);
}

QString corerenamer::remText(tPosition tpos, int pos, int count, const QString &selcText)
{
    int initialPos = 0;
    int remPos = pos;

    qDebug() << "BEFORE " << selcText;

    if (tpos == FROMRIGHT) {
        initialPos = selcText.length();
        remPos = initialPos - pos - count;
    }

    qDebug() << "AFTER " << QString(selcText).remove(remPos, count);

    return  QString(selcText).remove(remPos, count);
}

QString corerenamer::repText(const QString &sText, const QString &rText, const QString &selcText, Qt::CaseSensitivity s)
{
    return QString(selcText).replace(sText, rText, s);
}

void corerenamer::on_bAddFiles_clicked()
{
    QStringList list = QFileDialog::getOpenFileNames(this, "Add files", QDir::currentPath(), "*");
    if (list.count() > 0)
        addFiles(list);
}

bool corerenamer::setAddText()
{
    if (!ui->addTextCPos->text().count())
        return false;

    if (!ui->addTextEnter->text().count())
        return false;

    // Ignore odd (1, 3, 5, ...) items
    bool even = 1;
    foreach (QTableWidgetItem *text, ui->filesList->selectedItems()) {
        if (even) {
            QString t = text->text();
            if (ui->filesList->item(text->row(), 1)->text().count())
                t = ui->filesList->item(text->row(), 1)->text();
            QString addedT = addText(FROMLEFT, ui->addTextCPos->text().toInt(), ui->addTextEnter->text(), t);
            uStack->push(new ARRTextCommand(ui->filesList->item(text->row(), 1), addedT, ADD));
        }

        even = !even;
    }
    return true;
}

bool corerenamer::setRemText()
{
    if (!ui->removeTextCPos->text().count())
        return false;

    if (!ui->removeTextCCount->text().count())
        return false;

    // Ignore odd (1, 3, 5, ...) items
    bool even = 1;
    foreach (QTableWidgetItem *text, ui->filesList->selectedItems()) {
        if (even) {
            QString t = text->text();
            if (ui->filesList->item(text->row(), 1)->text().count())
                t = ui->filesList->item(text->row(), 1)->text();
            QString remedT = remText(FROMLEFT, ui->removeTextCPos->text().toInt(), ui->removeTextCCount->text().toInt(), t);
            uStack->push(new ARRTextCommand(ui->filesList->item(text->row(), 1), remedT, REMOVE));
        }

        even = !even;
    }
    return true;
}

bool corerenamer::setRepText()
{
    if (!ui->replaceTextText->text().count())
        return false;

    //if (!ui->replaceTextWith->text().count())
    //    return false;

    // Ignore odd (1, 3, 5, ...) items
    bool even = 1;
    foreach (QTableWidgetItem *text, ui->filesList->selectedItems()) {
        if (even) {
            QString t = text->text();
            if (ui->filesList->item(text->row(), 1)->text().count())
                t = ui->filesList->item(text->row(), 1)->text();
            QString replaT = repText(ui->replaceTextText->text(), ui->replaceTextWith->text(), t);
            uStack->push(new ARRTextCommand(ui->filesList->item(text->row(), 1), replaT, REPLACE));
        }

        even = !even;
    }
    return true;
}

void corerenamer::on_addTextbtn_clicked()
{
    if (ui->filesList->selectedItems().count() < 1)
        return;

    if (setAddText())
        ui->filesList->clearSelection();
}

void corerenamer::on_addTextAllbtn_clicked()
{
    ui->filesList->selectAll();

    if (setAddText())
        ui->filesList->clearSelection();
}

void corerenamer::on_removeTextTextbtn_clicked()
{
    if (ui->filesList->selectedItems().count() < 1)
        return;

    if (setRemText())
        ui->filesList->clearSelection();
}

void corerenamer::on_removeTextAllbtn_clicked()
{
    ui->filesList->selectAll();

    if (setRemText())
        ui->filesList->clearSelection();
}

void corerenamer::on_replaceTextbtn_clicked()
{
    if (ui->filesList->selectedItems().count() < 1)
        return;

    if (setRepText())
        ui->filesList->clearSelection();
}

void corerenamer::on_replaceTextAllbtn_clicked()
{
    ui->filesList->selectAll();

    if (setRepText())
        ui->filesList->clearSelection();
}

void corerenamer::casitivity(int i)
{
    if (ui->filesList->selectedItems().count() < 1)
        return;

    if (ui->addTextCPos->text().isNull())
        return;

    if (ui->addTextEnter->text().isNull())
        return;

    // Ignore odd (1, 3, 5, ...) items
    bool even = 1;
    foreach (QTableWidgetItem *text, ui->filesList->selectedItems()) {
        QString name = text->text();

        if (ui->filesList->item(text->row(), 1)->text().count())
            name = ui->filesList->item(text->row(), 1)->text();

        switch (i) {
        case 0:
            name = name.toUpper();
            break;
        case 1:
            qDebug() << name.at(0).toTitleCase();
            name = CapitalizeEachWord(name);
            break;
        case 2:
            name = name.toLower();
            break;
        default:
            name = name;
            break;
        }

        if (even) {
            uStack->push(new ARRTextCommand(ui->filesList->item(text->row(), 1), name, CASE));
        }

        even = !even;
    }
}

void corerenamer::on_upperCase_clicked()
{
    casitivity(0);
}

void corerenamer::on_capitalEach_clicked()
{
    casitivity(1);
}

void corerenamer::on_lowerCase_clicked()
{
    casitivity(2);
}

void corerenamer::on_formatc_activated(const QString &arg1)
{
    if (ui->filesList->selectedItems().count() < 1)
        return;

    // Ignore odd (1, 3, 5, ...) items
    bool even = 1;
    int count = 0;
    foreach (QTableWidgetItem *text, ui->filesList->selectedItems()) {
        if (even) {
            QString arg = "";
            count++;
            if (arg1.at(0) == '0') {
                if (arg1.at(1) == '0') {
                    arg = QString(text->row() > 8 ? text->row() > 98 ? "" : "0" : "00") + QString::number(count) + QString(arg1).remove(0, 3);
                } else {
                    arg = QString(text->row() > 8 ? "" : "0") + QString::number(count) + QString(arg1).remove(0, 2);
                }
            } else arg = QString::number(count) + QString(arg1).remove(0, 1);

            QString t = text->text();

            if (ui->filesList->item(text->row(), 1)->text().count())
                t = ui->filesList->item(text->row(), 1)->text();
            uStack->push(new ARRTextCommand(ui->filesList->item(text->row(), 1), addText(FROMLEFT, 0, arg, t), NUMBER));
        }

        even = !even;
    }
}

void corerenamer::on_browseSave_clicked()
{
    ui->savePath->setText(QFileDialog::getSaveFileName(this, "Save file"));
}

QString corerenamer::getOld(QTableWidgetItem *text) {
    QString ofile = ui->filesList->item(text->row(), 1)->data(Qt::UserRole).toString();
    if (ui->addOldOnly->isChecked()) {
        ofile = QFileInfo(ofile).fileName();
    }
    return ofile;
}

QString corerenamer::getNew(QTableWidgetItem *text) {
    QString ofile = ui->filesList->item(text->row(), 1)->data(Qt::UserRole).toString();
    QString cText = ui->filesList->item(text->row(), 1)->text();
    if (!cText.count()) return "";
    QString suffix = QFileInfo(ofile).suffix().isNull() ? "" : "." + QFileInfo(ofile).suffix();
    QString nfile = cText.isNull() ? "" : QFileInfo(ofile).path() + "/" + cText + suffix;
    if (ui->addNewOnly->isChecked()) {
        nfile = cText + QFileInfo(ofile).suffix();
    }
    return nfile;
}

void corerenamer::on_saveRenamed_clicked()
{
    QString filePath = ui->savePath->text();
    if (filePath.isNull() || !QDir(QFileInfo(filePath).path()).exists())
        return;

    QString textFile = "";
    // Ignore odd (1, 3, 5, ...) items
    bool even = 1;
    if (ui->gAddOldFileName->isChecked() && ui->gAddNewFileName->isChecked()) {
        ui->filesList->selectAll();
        even = 1;
        foreach (QTableWidgetItem *text, ui->filesList->selectedItems()) {
            //qDebug() << "SELECTED : " << text->text();
            if (even)
                textFile.append(getOld(text) + " " + ui->separatorC->currentText() + " " + getNew(text) + "\n");

            even = !even;
        }
    } else {
        if (ui->gAddNewFileName->isChecked()) {
            ui->filesList->selectAll();
            even = 1;
            foreach (QTableWidgetItem *text, ui->filesList->selectedItems()) {
                if (even) textFile.append(getNew(text) + "\n");
                even = !even;
            }
        } else if (ui->gAddOldFileName->isChecked()) {
            ui->filesList->selectAll();
            even = 1;
            foreach (QTableWidgetItem *text, ui->filesList->selectedItems()) {
                if (even) textFile.append(getOld(text) + "\n");
                even = !even;
            }
        } else {
            return;
        }
    }

    ui->filesList->clearSelection();

    QFile file(filePath);
    if (file.exists()) {
        long reply = QMessageBox::warning(this, "Warning", "File exists.\nDo you want to overwrite it.", QMessageBox::Yes, QMessageBox::No);
        if (reply == QMessageBox::No) return;
    }
    file.open(QIODevice::WriteOnly);
    file.write(textFile.toLatin1());
    file.close();
}

void corerenamer::on_bUndo_clicked()
{
    uStack->undo();
}

void corerenamer::on_bRedo_clicked()
{
    uStack->redo();
}

void corerenamer::on_rename_clicked()
{
    long reply = QMessageBox::information(this, "Permission", "Are you sure to rename those files?\nIf you once rename them it is not possible to go back...",
                                          QMessageBox::Yes, QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        ui->filesList->selectAll();
        // Ignore odd (1, 3, 5, ...) items
        bool even = 1;
        foreach (QTableWidgetItem *text, ui->filesList->selectedItems()) {
            if (even) {
                if (ui->filesList->item(text->row(), 1)->text().count()) {
                    // FIXME : IF FILE EXISTS THEN DO A ACTION
                    QFile::rename(ui->filesList->item(text->row(), 1)->data(Qt::UserRole).toString(), getNew(text));
                    ui->filesList->item(text->row(), 1)->setData(Qt::UserRole, getNew(text));
                }
            }
            even = !even;
        }

        uStack->clear();
        ui->filesList->clearSelection();
    }
}

void corerenamer::on_bRefreshList_clicked()
{
    ui->filesList->clear();
    uStack->clear();
}
