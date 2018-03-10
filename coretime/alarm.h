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

#ifndef ALARM_H
#define ALARM_H

#include <QObject>
#include <QString>
#include <QMediaPlayer>
#include <QTimer>


class Alarm : public QObject
{
    Q_OBJECT

public:
    explicit Alarm(QObject *parent = 0);
    void Start(bool useCustom);
    void Stop();
    bool isPlaying();
    bool canResume;
    void SetCustomPath(QString);
    bool UsingCustomPath;

private:
    QMediaPlayer * media;
    QString _DefaultPath;
    QString _CustPath;
    QTimer *_Pause;
    bool _isPlaying;
     
public slots:
    void RepeatAllTheThings(QMediaPlayer::MediaStatus);
    void Resume();
    void SetVolume(int);
    
};

#endif // ALARM_H
