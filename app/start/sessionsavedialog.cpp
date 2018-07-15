#include "sessionsavedialog.h"
#include "ui_sessionsavedialog.h"

#include <QSettings>
#include <QDir>
#include <QDebug>

sessionSaveDialog::sessionSaveDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::sessionSaveDialog)
{
    ui->setupUi(this);

    sName = "";

    QSettings session(QDir::homePath() + "/.config/coreBox/Sessions", QSettings::IniFormat);
    QStringList group = session.childGroups();
    foreach (QString s, group) {
        session.beginGroup(s);
        QStringList gl = session.childGroups();
        foreach (QString s, gl) {
            nameList.append(s);
        }
        session.endGroup();
    }

    ui->nameIt->setVisible(false);
    ui->confirm->setVisible(true);

    ui->cMessage->setText("Do you want to save this session ?");

    ui->ok->setText("Yes");
    ui->cancel->setText("No");

    connect(ui->cancel, &QPushButton::clicked, this, &sessionSaveDialog::close);
}

sessionSaveDialog::~sessionSaveDialog()
{
    delete ui;
}

void sessionSaveDialog::on_ok_clicked()
{
    if (ui->confirm->isVisible()) {
        ui->ok->setText("OK");
        ui->cancel->setText("Cancel");

        ui->nameIt->setVisible(true);
        ui->confirm->setVisible(false);

        ui->sessionName->setFocus();
    } else if (ui->nameIt->isVisible()) {
        if (ui->sessionName->text().count()) {
            sName = ui->sessionName->text();
            accept();
            emit nameOk();
        }
    }
}

void sessionSaveDialog::on_sessionName_textChanged(const QString &arg1)
{
    if (arg1.count()) {
        if (!nameList.contains(arg1)) {
            ui->ok->setEnabled(true);
        } else
            ui->ok->setEnabled(false);
    } else
        ui->ok->setEnabled(false);
}
