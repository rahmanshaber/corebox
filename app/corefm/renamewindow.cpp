#include "renamewindow.h"
#include "ui_renamewindow.h"

#include "corebox/globalfunctions.h"

#include <QPixmap>

RenameWindow::RenameWindow(QFileInfo iFile, QPixmap pix, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RenameWindow)
{
    ui->setupUi(this);
    m_iFile = iFile;

    setWindowFlags(Qt::Dialog | Qt::Popup);
    setAttribute(Qt::WA_NativeWindow);

    setWindowIcon(geticon(m_iFile.filePath()));
    setWindowTitle("Rename \"" + m_iFile.fileName() + "\"");
    ui->newName->setText(m_iFile.fileName());
    ui->pic->setPixmap(pix);

    connect(ui->cancel, &QPushButton::clicked, this, &RenameWindow::close);
}

RenameWindow::~RenameWindow()
{
    delete ui;
}

void RenameWindow::on_ok_clicked()
{
    if (ui->newName->text().count()) {
        if (QFile::rename(m_iFile.filePath(), m_iFile.path() + "/" + ui->newName->text())) {
            messageEngine("File Renamed Successfully.", MessageType::Info);
            close();
        } else {
            messageEngine("File not Renamed.", MessageType::Warning);
            close();
        }
    }
}
