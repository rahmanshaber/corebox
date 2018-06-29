#include "wnotes.h"
#include "ui_wnotes.h"

#include <QSettings>
#include <QDir>

wNotes::wNotes(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::wNotes)
{
    ui->setupUi(this);
    collectNotes();
}

wNotes::~wNotes()
{
    delete ui;
}

void wNotes::collectNotes()
{
    QSettings notes(QDir::homePath() + "/.config/coreBox/Notes", QSettings::IniFormat);
    notes.beginGroup("Notes");
    ui->plainTextEdit->setPlainText(notes.value("1").toString());
    ui->plainTextEdit_2->setPlainText(notes.value("2").toString());
    notes.endGroup();
    notes.sync();
}

bool wNotes::saveNotes()
{
    QSettings notes(QDir::homePath() + "/.config/coreBox/Notes", QSettings::IniFormat);
    if (notes.status() == QSettings::NoError) {
        notes.beginGroup("Notes");
        notes.setValue("1", ui->plainTextEdit->toPlainText());
        notes.setValue("2", ui->plainTextEdit_2->toPlainText());
        notes.endGroup();
        notes.sync();
        return true;
    } else {
        return false;
    }
}

void wNotes::closeEvent(QCloseEvent *event)
{
    event->ignore();
    if (saveNotes()) event->accept();
    else event->ignore();
}
