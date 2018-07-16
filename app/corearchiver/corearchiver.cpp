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

#include "corearchiver.h"
#include "ui_corearchiver.h"


corearchiver::corearchiver(QWidget *parent) :QWidget(parent),ui(new Ui::corearchiver)
{
    ui->setupUi(this);

    // set stylesheet from style.qrc
    setStyleSheet(getStylesheetFileContent(":/appStyle/style/CoreArchiver.qss"));

    startsetup();
}

corearchiver::~corearchiver()
{
    delete ui;
}

void corearchiver::startsetup()
{
    connect( ui->nameLE, SIGNAL( textChanged( QString ) ), this, SLOT( updateFileName( QString ) ) );
    connect( ui->locationTB, SIGNAL( clicked() ), this, SLOT( updateDirName() ) );

    //add the archive formate type
    ui->formatsCB->addItems( QStringList() << ".zip" << ".tar.xz" << ".tar.gz" << ".tar.bz2" << ".7z");
    ui->formatsCB->addItems( QStringList() << ".tar" << ".iso" << ".cpio" << ".shar" << ".tar.Z" << ".ar" << ".xar" );
    connect( ui->formatsCB, SIGNAL( currentIndexChanged( QString ) ), this, SLOT( updateFormat( QString ) ) );

    connect( ui->createArc, &QToolButton::clicked, [this](){
        compress(filePathList, !workingDir.isEmpty() ? QDir(workingDir) : QDir::current());
    } );

    connect( ui->cancel, SIGNAL( clicked() ), this, SLOT(close()));

    ui->locationLE->setEnabled(false);
    format = ui->formatsCB->currentText();
    ui->nameLE->setText(archiveName);
    ui->nameLE->setFocus();
}

/*
 * Info :
 *
 * location = where the compressed file will be located
 * working dir = where the file is taken from
 *
 *
*/

void corearchiver::setFilename(const QString &fileName)
{
    archiveName = fileName;
    ui->nameLE->setText(archiveName);
}

void corearchiver::setFolderPath(const QString &path)
{
    workingDir = path;
    location = path;
}

void corearchiver::compress(const QStringList &archiveList , const QDir &currentDir)
{
    ui->nameLE->setEnabled(false);
    ui->locationLE->setEnabled(false);
    ui->locationTB->setEnabled(false);
    ui->formatsCB->setEnabled(false);
    ui->createArc->setEnabled(false);
    ui->cancel->setEnabled(false);

    QDir::setCurrent(currentDir.path());

    qDebug() << "Current Dir : " << QDir::currentPath() << "\nGiven Dir : " << currentDir.path();

    if (archiveName.isEmpty()) return;

    QString name = archiveName;
    if (!archiveName.endsWith(format))
        name += format;

    LibArchive *arc = new LibArchive(QDir(location).filePath(name));

    if ( arc == NULL )
        return;

    qDebug() << "Info " << QDir(location).filePath(name);
    qDebug() << "Format : " << format;
    qDebug() << "Archive Name : " << name;
    qDebug() << "Working Dir : " << currentDir.path();
    qDebug() << "List Files : " << archiveList;
    arc->setWorkingDir(currentDir.path());
    arc->updateInputFiles( archiveList );
    QFuture<void> f = QtConcurrent::run( arc, &LibArchive::create );
    QFutureWatcher<void> *fw = new QFutureWatcher<void>();
    fw->setFuture(f);
    connect(fw, &QFutureWatcher<void>::finished, [&]() {
        deleteLater();
    });
}

/**
 * @brief coreArc::extract Extract the archive at the archive path
 * @param archive Archive file path
 */
void corearchiver::extract(const QString &archiveFilePath , const QDir &dest)
{
    const QString destP = dest.path();
    // Create the dest folder if it does nor exist
    if (!QDir(destP).exists())
        QDir::current().mkdir( destP );

    LibArchive arc( archiveFilePath );
    arc.setDestination( destP );
    QFuture<void> f = QtConcurrent::run( arc, &LibArchive::extract );
    QFutureWatcher<void> *fw = new QFutureWatcher<void>();
    fw->setFuture(f);
    connect(fw, &QFutureWatcher<void>::finished, [&]() {
        deleteLater();
    });
}

void corearchiver::updateFileName(const QString &fn)
{
    archiveName = QString( fn );
}

void corearchiver::updateDirName()
{
    const QString loc = QFileDialog::getExistingDirectory( this, "NewBreeze - Choose Directory", QDir::currentPath() );
    if ( not loc.isEmpty() ) {
        ui->locationLE->setEnabled(true);
        ui->locationLE->setText( loc );
        location = loc;
    }
}

void corearchiver::updateFormat(const QString &fmt )
{
    format = QString( fmt );
}
