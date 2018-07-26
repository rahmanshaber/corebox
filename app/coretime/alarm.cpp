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

#include "alarm.h"
#include "fileio.h"

#include <QMediaPlayer>
#include <QString>
#include <QDir>


Alarm::Alarm(QObject *parent) :
    QObject(parent)
{
    media = new QMediaPlayer(this);
    
    this->_DefaultPath=QDir::tempPath()+"/sound.ogg";
    this->_isPlaying=false;
    this->_Pause=new QTimer(this);
    this->canResume=true;

    connect(this->_Pause,SIGNAL(timeout()),this,SLOT(Resume()));
}

void Alarm::Start(bool useCustom)
{
    if(useCustom)
    {
        media->setMedia(QUrl::fromLocalFile(this->_CustPath));
        this->UsingCustomPath=true;
    }else{
        FileIO::ExtractAudio();
        media->setMedia(QUrl::fromLocalFile(this->_DefaultPath));
        this->UsingCustomPath=false;
    }
    media->play();
    connect(media,SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),this,SLOT(RepeatAllTheThings(QMediaPlayer::MediaStatus)));
    this->_isPlaying=true;
}

void Alarm::Stop()
{
    media->stop();
    this->_Pause->start(60000);
    this->_isPlaying=false;
}

void Alarm::RepeatAllTheThings(QMediaPlayer::MediaStatus state)
{
    if(state==QMediaPlayer::EndOfMedia)
    {
        if(this->UsingCustomPath)
        {
            media->setMedia(QUrl::fromLocalFile(this->_CustPath));
        }else{
            media->setMedia(QUrl::fromLocalFile(this->_DefaultPath));
        }
        media->play();
    }
}

bool Alarm::isPlaying()
{
    return this->_isPlaying;
}

void Alarm::SetCustomPath(QString CustPath)
{
    this->_CustPath=CustPath;
}

void Alarm::Resume()
{
    this->canResume=true;
    this->_Pause->stop();
}


void Alarm::SetVolume(int Volume)
{
    media->setVolume(Volume);
}

