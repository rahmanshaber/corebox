#include "corearchiver.h"
#include "ui_corearchiver.h"

#include <QtConcurrent>


corearchiver::corearchiver(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::corearchiver)
{
    ui->setupUi(this);

    connect( ui->nameLE, SIGNAL( textChanged( QString ) ), this, SLOT( updateFileName( QString ) ) );
    connect( ui->locationTB, SIGNAL( clicked() ), this, SLOT( updateDirName() ) );

    //add the archive formate type
    ui->formatsCB->addItems( QStringList() << ".zip" << ".tar.xz" << ".tar.gz" << ".tar.bz2" << ".7z");
    ui->formatsCB->addItems( QStringList() << ".tar" << ".iso" << ".cpio" << ".shar" << ".tar.Z" << ".ar" << ".xar" );
    connect( ui->formatsCB, SIGNAL( currentIndexChanged( QString ) ), this, SLOT( updateFormat( QString ) ) );

    connect( ui->createArc, &QToolButton::clicked, [this](){
//        qDebug() << "Archive Destination : " << location << "\nFrom line edit setting : " << ui->locationLE->text();
        compress(filePathList, !workingDir.isEmpty() ? QDir(workingDir) : QDir::current());
    } );

    connect( ui->cancel, SIGNAL( clicked() ), this, SLOT(close()));

    format = ui->formatsCB->currentText();
    ui->nameLE->setText(archiveName);
}

corearchiver::~corearchiver()
{
    delete ui;
}

void corearchiver::compress(QStringList archiveList , QDir currentDir)
{
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
void corearchiver::extract(QString archiveFilePath , QDir dest)
{
    QString destP = dest.path();
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

void corearchiver::updateFileName( QString fn )
{
    archiveName = QString( fn );
}

void corearchiver::updateDirName()
{
    QString loc = QFileDialog::getExistingDirectory( this, "NewBreeze - Choose Directory", QDir::currentPath() );
    if ( not loc.isEmpty() ) {
        ui->locationLE->setText( loc );
        location = loc;
    }
}

void corearchiver::updateFormat( QString fmt )
{
    format = QString( fmt );
}
