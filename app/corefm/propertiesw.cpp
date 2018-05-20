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

#include <sys/stat.h>
#include <QImageReader>
#include <QMimeDatabase>
#include <QMediaMetaData>

#include "corefm.h"
#include "../corebox/globalfunctions.h"


propertiesw::propertiesw(const QString paths,QWidget *parent) :QWidget(parent),ui(new Ui::propertiesw)
{
    qDebug() << "propertiesw opening";
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

    connect(ui->ok,SIGNAL(pressed()),this,SLOT(close()));
    this->setAttribute(Qt::WA_DeleteOnClose,1);
}

propertiesw::~propertiesw(){

    qDebug() << "propertiesw closing";
    delete ui;
}

void propertiesw::general(){

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
    if(info.isExecutable() && !info.isDir()){
        ui->executable->setText("File is Executable");
        ui->executableB->setChecked(1);
    }
}

void propertiesw::details(){

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

void propertiesw::permission(){

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

    if(info.isDir()){
        ui->executableB->setVisible(0);
    }
}

void propertiesw::partition(const QString path){

    QString t= formatSize(QStorageInfo(path).bytesTotal());
    QString f= formatSize(QStorageInfo(path).bytesFree());
//    QString s = QString::number((double) t - f);

    ui->sizefree->setText("Free : " + f);
    ui->sizetotal->setText("Total : " + t);
}

void propertiesw::checkboxesChanged(){

    ui->permissionsNumeric->setText(QString("%1%2%3").arg(ui->ownerRead->isChecked()*4 + ui->ownerWrite->isChecked()*2 + ui->ownerExec->isChecked())
                                .arg(ui->groupRead->isChecked()*4 + ui->groupWrite->isChecked()*2 + ui->groupExec->isChecked())
                                .arg(ui->otherRead->isChecked()*4 + ui->otherWrite->isChecked()*2 + ui->otherExec->isChecked()));
}

void propertiesw::numericChanged(QString text){

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
    if(checked){
        QProcess p1;
        QString commd = "chmod a+x \"" + pathName + "\"";
        p1.start(commd.toLatin1());
        p1.waitForFinished();
    }
    else if(!checked){
        QProcess p1;
        QString commd = "chmod -x \"" + pathName + "\"";
        p1.start(commd.toLatin1());
        p1.waitForFinished();
    }
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
    right << info.fileName() << formatSize(info.size()) << info.suffix().toUpper()
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
