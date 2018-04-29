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

//#include <QtConcurrent/QtConcurrent>
//#include <sys/vfs.h>
#include <sys/stat.h>
#include <QImageReader>
#include <QMimeDatabase>

#include "corefm.h"
#include "../corebox/globalfunctions.h"


propertiesw::propertiesw(QString paths,QWidget *parent) :QWidget(parent),ui(new Ui::propertiesw)
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

    if(!info.isDir() && info.isFile()){
        ui->typeIcon->setIcon(geticon(info.fileName()));
        ui->type->setText(littleinfo + " , " + extrainfo );
    }
    if(info.isDir() && !info.isFile()){
        ui->type->setText("Folder");
        ui->typeIcon->setIcon(QIcon(":/icons/folder.svg"));
    }

    ui->fileName->setText(info.fileName());
    ui->size->setText(getFileSize(pathName));
    ui->location->setText(info.path());

    ui->modified->setText(info.lastModified().toString());
    ui->created->setText(info.birthTime().toString());

    ui->owner->setText(info.owner());
    ui->group->setText(info.group());
    ui->executable->setText("File is not Executable");
    if(info.isExecutable() && !info.isDir()){
        ui->executable->setText("File is Executable");
        ui->executableB->setChecked(1);
    }
}

void propertiesw::details(){

    QImageReader reader(pathName);
    QString type = info.suffix();
    ui->imagetype->setText(type);
    const QImage image = reader.read();
    ui->dimensions->setText(QString::number(image.width()) + " x "+ QString::number(image.height()));
    ui->bitplanecount->setText(QString::number(image.bitPlaneCount()));
    ui->imagewidth->setText(QString::number(image.width()) + " px");
    ui->imageheight->setText(QString::number(image.height()) + " px") ;

    if(image.bitPlaneCount() == 0){
        ui->propertiestab->removeTab(1);
    }
}

void propertiesw::permission(){

    connect(ui->otherRead,SIGNAL(clicked(bool)),this,SLOT(checkboxesChanged()));
    connect(ui->ownerWrite,SIGNAL(clicked(bool)),this,SLOT(checkboxesChanged()));
    connect(ui->ownerExec,SIGNAL(clicked(bool)),this,SLOT(checkboxesChanged()));

    connect(ui->groupRead,SIGNAL(clicked(bool)),this,SLOT(checkboxesChanged()));
    connect(ui->groupWrite,SIGNAL(clicked(bool)),this,SLOT(checkboxesChanged()));
    connect(ui->groupExec,SIGNAL(clicked(bool)),this,SLOT(checkboxesChanged()));

    connect(ui->otherRead,SIGNAL(clicked(bool)),this,SLOT(checkboxesChanged()));
    connect(ui->otherWrite,SIGNAL(clicked(bool)),this,SLOT(checkboxesChanged()));
    connect(ui->otherExec,SIGNAL(clicked(bool)),this,SLOT(checkboxesChanged()));

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

void propertiesw::partition(QString path){

    QString t= formatSize(QStorageInfo(path).bytesTotal());
    QString f= formatSize(QStorageInfo(path).bytesFree());
//    QString s = QString::number((double) t - f);

    ui->patitionN->setText(QStorageInfo(path).name());
    ui->sizefree->setText("Free : " + f);
    ui->sizetotal->setText("Total : " + t);
}

QIcon propertiesw::geticon(const QString &filename){

    QMimeDatabase mime_database;
    QIcon icon;
    QList<QMimeType> mime_types = mime_database.mimeTypesForFileName(filename);
    for (int i=0; i < mime_types.count() && icon.isNull(); i++)
      icon = QIcon::fromTheme(mime_types[i].iconName());

    if (icon.isNull())
      return QApplication::style()->standardIcon(QStyle::SP_FileIcon);
    else
      return icon;
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
        QString commd = "chmod a+x " + pathName;
        qDebug()<< commd;
        p1.start(commd.toLatin1());
        p1.waitForFinished();
    }
    else if(!checked){
        QProcess p1;
        QString commd = "chmod -x " + pathName;
        qDebug()<< commd;
        p1.start(commd.toLatin1());
        p1.waitForFinished();
    }
}
