/*
CoreArchiver is free software; you can redistribute it and/or
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

#include <QtWidgets>
#include <QtConcurrent>


corearchiver::corearchiver(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("CoreArchiver - Create an Archive" );
    setWindowIcon( QIcon::fromTheme("archive") );

    /* Name Lyt */
    nameLE = new QLineEdit( this );
    nameLE->setPlaceholderText( "Type the archive name here" );
    connect( nameLE, SIGNAL( textChanged( QString ) ), this, SLOT( updateFileName( QString ) ) );

    QLabel *nameLbl = new QLabel( "Archive &Name:" );
    nameLbl->setBuddy( nameLE );

    QHBoxLayout *nLyt = new QHBoxLayout();
    nLyt->addWidget( nameLbl );
    nLyt->addWidget( nameLE );

    /* Location Lyt */
    locationLE = new QLineEdit( this );
    locationLE->setPlaceholderText( "Click browse to select where the archive will be stored" );
    locationLE->setDisabled( true );

    QLabel *locationLbl = new QLabel( "&Location:", this );
    locationLbl->setBuddy( locationLE );

    QToolButton *locationTB = new QToolButton( this );
    locationTB->setIcon( QIcon::fromTheme( "folder" ) );
    connect( locationTB, SIGNAL( clicked() ), this, SLOT( updateDirName() ) );

    QHBoxLayout *lLyt = new QHBoxLayout();
    lLyt->addWidget( locationLbl );
    lLyt->addWidget( locationLE );
    lLyt->addWidget( locationTB );

    /* Format Layout */
    QComboBox *formatsCB = new QComboBox( this );
    formatsCB->addItems( QStringList() << ".zip" << ".tar.xz" << ".tar.gz" << ".tar.bz2" << ".7z");
    formatsCB->addItems( QStringList() << ".tar" << ".iso" << ".cpio" << ".shar" << ".tar.Z" << ".ar" << ".xar" );
    connect( formatsCB, SIGNAL( currentIndexChanged( QString ) ), this, SLOT( updateFormat( QString ) ) );

    QLabel *formatsLbl = new QLabel( "&Format:", this );
    formatsLbl->setBuddy( formatsCB );

    /* WHAT IS ADVACNED? WHY DID I ADD THIS? */
    QCheckBox *advanced = new QCheckBox( "Ad&vanced", this );
    advanced->setDisabled( true );
    advanced->setStyleSheet( "color:gray;");
    advanced->hide();

    QHBoxLayout *fLyt = new QHBoxLayout();
    fLyt->addWidget( advanced );
    fLyt->addStretch();
    fLyt->addWidget( formatsLbl );
    fLyt->addWidget( formatsCB );

    /* Buttons */
    QToolButton *createArc = new QToolButton( this );
    createArc->setText("Create &Archive");
    createArc->setObjectName("okBtn");

    connect( createArc, &QToolButton::clicked, [this](){
        qDebug() << "Archive Destination : " << location << "\nFrom line edit setting : " << locationLE->text();
        compress(filePathList, !workingDir.isEmpty() ? QDir(workingDir) : QDir::current());
    } );

    QToolButton *cancel = new QToolButton(this);
    cancel->setText("&Cancel" );
    cancel->setObjectName( "cancelBtn" );
    connect( cancel, SIGNAL( clicked() ), this, SLOT(close()));

    QHBoxLayout *bLyt = new QHBoxLayout();
    bLyt->addStretch();
    bLyt->addWidget( createArc );
    bLyt->addWidget(cancel);

    /* Final Layout */
    QVBoxLayout *baseLyt = new QVBoxLayout();
    baseLyt->addLayout( nLyt );
    baseLyt->addLayout( lLyt );
    baseLyt->addLayout( fLyt );
    //baseLyt->addWidget( Separator::horizontal() );
    baseLyt->addLayout( bLyt );

    setLayout( baseLyt );

    format = formatsCB->currentText();
}

corearchiver::~corearchiver()
{
    delete nameLE;
    delete locationLE;
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
        locationLE->setText( loc );
        location = loc;
    }
}

void corearchiver::updateFormat( QString fmt )
{
    format = QString( fmt );
}
