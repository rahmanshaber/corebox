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

#include "propertiesw.h"
#include "ui_propertiesw.h"

#include "corefm.h"


propertiesw::propertiesw(const QString paths,QWidget *parent) :QWidget(parent),ui(new Ui::propertiesw)
{
    ui->setupUi(this);

    pathName = paths;
    info = QFileInfo(pathName);

    permission();
    general();
    details();
    partition(pathName);
    show();
    int x = screensize().width()  * .27;
    int y = screensize().height() * .6;
    this->setFixedSize(x,y);

    connect( ui->close, SIGNAL( clicked() ), this, SLOT(close()));
    this->setAttribute(Qt::WA_DeleteOnClose,1);
}

propertiesw::~propertiesw()
{
    delete ui;
}

void propertiesw::general()
{
    QFile file(pathName);
    QMimeDatabase db;
    QMimeType mime = db.mimeTypeForFile(file, QMimeDatabase::MatchContent);
    QString littleinfo = info.suffix();
    QString extrainfo = mime.name();

    this->setWindowTitle("Properties - " + info.fileName() );

    ui->typeIcon->setIcon(geticon(info.filePath()));

    if(info.isFile()){
        ui->type->setText(littleinfo + " , " + extrainfo );
    }
    if(info.isDir()){
        ui->type->setText("Directory");
    }

    ui->fileName->setText(info.fileName());
    ui->size->setText(getFileSize(pathName));
    ui->location->setText(info.path());

    ui->modified->setText(info.lastModified().toString());
    //ui->created->setText(info.birthTime().toString());

    ui->owner->setText(info.owner());
    ui->group->setText(info.group());
    ui->executable->setText("File is not Executable");
    if(info.isExecutable() == true){
        ui->executable->setText("File is Executable");
        ui->executableB->setChecked(1);
    }
}

void propertiesw::details()
{
    QStringList image,media;
    image << "jpg" << "jpeg" << "png" << "bmp" << "ico" << "svg" << "gif";
    media << "webm" << "mkv" << "flv" << "avi" << "mov" << "m4a"
             << "mp4" << "3gp" << "wav" << "mp3" << "ogg" << "flac" ;

    QString suffix = info.suffix();

    //image
    if (image.contains(suffix, Qt::CaseInsensitive)) {
        detailimage(pathName);
    }

    //media
    else if (media.contains(suffix, Qt::CaseInsensitive)) {
        m_player = new QMediaPlayer(this);
        m_player->setMedia(QUrl::fromLocalFile(pathName));
        connect(m_player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),
                this, SLOT(detailmedia(QMediaPlayer::MediaStatus)));
    }

    //not supported
    else{
        ui->propertiestab->removeTab(1);
    }
}

void propertiesw::permission()
{
    checkboxesChanged();
    connect(ui->permissionsNumeric, SIGNAL(textChanged(QString)), this,SLOT(numericChanged(QString)));

    struct stat perms;
    stat(pathName.toLocal8Bit(), &perms);
    permString = QString("%1%2%3").arg(((perms.st_mode & S_IRWXU) >> 6)).arg(((perms.st_mode & S_IRWXG) >> 3)).arg((perms.st_mode & S_IRWXO));

    ui->permissionsNumeric->setText(permString);

    QRegExp input("^[0-7]*$");
    QValidator *permNumericValidator = new QRegExpValidator(input, this);
    ui->permissionsNumeric->setValidator(permNumericValidator);
    ui->permissionsNumeric->setMaxLength(3);

    int ret = chmod(pathName.toLocal8Bit(),permString.toInt(0,8));
    if(ret) ui->permissions->setDisabled(1);

    ui->permissions->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->permissions->setFocusPolicy(Qt::NoFocus);

    if(isExecutable(info.filePath()) == false){
        ui->executableB->setVisible(0);
    }
}

void propertiesw::partition(const QString path)
{
    // Function from globalfunctions.cpp
    QString t= formatSize(QStorageInfo(path).bytesTotal());
    QString f= formatSize(QStorageInfo(path).bytesFree());
//    QString s = QString::number((double) t - f);

    ui->sizefree->setText("Free : " + f);
    ui->sizetotal->setText("Total : " + t);
}

void propertiesw::checkboxesChanged()
{
    ui->permissionsNumeric->setText(QString("%1%2%3").arg(ui->ownerRead->isChecked()*4 + ui->ownerWrite->isChecked()*2 + ui->ownerExec->isChecked())
                                .arg(ui->groupRead->isChecked()*4 + ui->groupWrite->isChecked()*2 + ui->groupExec->isChecked())
                                .arg(ui->otherRead->isChecked()*4 + ui->otherWrite->isChecked()*2 + ui->otherExec->isChecked()));
}

void propertiesw::numericChanged(QString text)
{
    if(text.count() != 3) return;

    int owner = QString(text.at(0)).toInt();
    ui->ownerRead->setChecked(owner / 4);
    ui->ownerWrite->setChecked((owner - owner / 4 * 4- owner % 2));
    ui->ownerExec->setChecked(owner % 2);

    int group = QString(text.at(1)).toInt();
    ui->groupRead->setChecked(group / 4);
    ui->groupWrite->setChecked((group - group / 4 * 4 - group % 2));
    ui->groupExec->setChecked(group % 2);

    int other = QString(text.at(2)).toInt();
    ui->otherRead->setChecked(other / 4);
    ui->otherWrite->setChecked((other - other / 4 * 4 - other % 2));
    ui->otherExec->setChecked(other % 2);
}

void propertiesw::on_executableB_clicked(bool checked)
{
    ui->ownerExec->setChecked(checked);
    ui->groupExec->setChecked(checked);
    ui->otherExec->setChecked(checked);

    QString path = info.filePath();
    //Q_FOREACH( QString path, pathsList ) {
        QFile::Permissions perms = NULL;

        if ( ui->ownerRead->isChecked() )
            perms |= ( ( ui->ownerRead->checkState() == Qt::Checked ) ? QFile::ReadOwner : ( QFile::permissions( path ) & QFile::ReadOwner ) );

        if ( ui->ownerWrite->isChecked() )
            perms |= ( ( ui->ownerWrite->checkState() == Qt::Checked ) ? QFile::WriteOwner : ( QFile::permissions( path ) & QFile::WriteOwner ) );

        if ( ui->ownerExec->isChecked() )
            perms |= ( ( ui->ownerExec->checkState() == Qt::Checked ) ? QFile::ExeOwner : ( QFile::permissions( path ) & QFile::ExeOwner ) );

        if ( ui->groupRead->isChecked() )
            perms |= ( ( ui->groupRead->checkState() == Qt::Checked ) ? QFile::ReadGroup : ( QFile::permissions( path ) & QFile::ReadGroup ) );

        if ( ui->groupWrite->isChecked() )
            perms |= ( ( ui->groupWrite->checkState() == Qt::Checked ) ? QFile::WriteGroup : ( QFile::permissions( path ) & QFile::WriteGroup ) );

        if ( ui->groupExec->isChecked() )
            perms |= ( ( ui->groupExec->checkState() == Qt::Checked ) ? QFile::ExeGroup : ( QFile::permissions( path ) & QFile::ExeGroup ) );

        if ( ui->otherRead->isChecked() )
            perms |= ( ( ui->otherRead->checkState() == Qt::Checked ) ? QFile::ReadOther : ( QFile::permissions( path ) & QFile::ReadOther ) );

        if ( ui->otherWrite->isChecked() )
            perms |= ( ( ui->otherWrite->checkState() == Qt::Checked ) ? QFile::WriteOther : ( QFile::permissions( path ) & QFile::WriteOther ) );

        if ( ui->otherExec->isChecked() )
            perms |= ( ( ui->otherExec->checkState() == Qt::Checked ) ? QFile::ExeOther : ( QFile::permissions( path ) & QFile::ExeOther ) );

        QFile::setPermissions( path, perms );

        checkboxesChanged();
}

void propertiesw::detailimage(const QString imagepath)
{
    QImageReader reader(imagepath);
    const QImage image = reader.read();
    QFileInfo info(imagepath);

    QStringList left;
    QStringList right;
    left << "Name" << "Size" << "Type" << "Dimensions" << "Bitplane Count"
         << "Width" << "Height" ;
    right << info.fileName() << formatSize(info.size()) << info.suffix().toUpper() // Function from globalfunctions.cpp
          << QString::number(image.width()) + " x "+ QString::number(image.height())
          << QString::number(image.bitPlaneCount()) << QString::number(image.width()) + " pixels"
          << QString::number(image.height()) + " pixels" ;

    QStringListModel *infoModel = new QStringListModel(fStringList(left, right, ui->detail->font()));
    ui->detail->setModel(infoModel);
    ui->detail->setFocusPolicy(Qt::NoFocus);
}

void propertiesw::detailmedia(QMediaPlayer::MediaStatus status)
{
    if (status == QMediaPlayer::LoadedMedia){

        QStringList left;
        QStringList right;
        QStringList metadatalist = m_player->availableMetaData();
        int list_size = metadatalist.size();
        QString metadata_key;
        QVariant var_data;

        for (int indx = 0; indx < list_size; indx++)
        {
            metadata_key  = metadatalist.at(indx);
            var_data      = m_player->metaData(metadata_key);

            left << metadata_key;
            right << var_data.toString();
        }

        float pi = m_player->duration()/60000.0;

        left << "Duration";
        right << QString::number(pi) + " mins";

        QStringListModel *infoModel = new QStringListModel(fStringList(left, right, ui->detail->font()));
        ui->detail->setModel(infoModel);
        ui->detail->setFocusPolicy(Qt::NoFocus);
    }
}


bool propertiesw::isExecutable( const QString path )
{
    QStringList type;
    type << "so" << "o" << "sh" << "deb" << "rpm" << "tar.gz"
             << "tar" << "gz" << "ko" << "AppImage";

    QString suffix = info.suffix();

    if (type.contains(suffix, Qt::CaseInsensitive))
        return true;


    return false;
}
