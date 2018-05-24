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

#ifndef COREPLAYER_H
#define COREPLAYER_H

#include <QWidget>
#include <QAbstractItemView>
#include <QVideoProbe>
#include <QVideoWidget>
#include <QAudioProbe>
#include <QStandardItemModel>
#include <QMediaPlayer>
#include <QCloseEvent>
#include <QModelIndex>
#include <QMediaPlaylist>

#include "../corebox/corebox.h"
#include "../corebox/globalfunctions.h"


namespace Ui {
class coreplayer;
}

class QAbstractItemView;
class QLabel;
class QVideoProbe;
class QVideoWidget;
class QAudioProbe;
class QStandardItemModel;


class coreplayer : public QWidget
{
    Q_OBJECT

public:
    explicit coreplayer(QWidget *parent = 0);
    ~coreplayer();

    void openPlayer(const QString path);

    bool isPlayerAvailable() const;
    QMediaPlayer::State state() const;

protected:
    void closeEvent(QCloseEvent *event);

public slots:
    void setState(QMediaPlayer::State state);

private:
    Ui::coreplayer *ui;

    QStringList getAudios(const QString path);
    QStringList getVideos(const QString path);
    QStringList videomimes;
    QStringList audiomimes;

    void audioMimes();
    void videoMimes();
    void setTrackInfo(const QString &info);
    void setStatusInfo(const QString &info);
    void handleCursor(QMediaPlayer::MediaStatus status);
    void updateDurationInfo(qint64 currentInfo);
    void setFolder(const QString &foldername);
    void play(int index);
    void setCurrentIndex(int currentIndex);
    void shotcuts();

    int mNumberOfFiles;
    QMediaPlayer::State playerState;
    QMediaPlayer *player;
    QVideoProbe *videoProbe;
    QAudioProbe *audioProbe;
    QAbstractItemView *medialist;
    qint64 duration;
    QStandardItemModel *mModel;

    QString folderpath;
    QString filepath;

    void creatPlayList(const QString path);
private slots:
    void seekLeft();
    void seekRight();
    void volumnUP();
    void volumnDown();

    void durationChanged(qint64 duration);
    void positionChanged(qint64 progress);
    void seek(int seconds);
    void statusChanged(QMediaPlayer::MediaStatus status);
    void bufferingProgress(int progress);
    void displayErrorMessage();

    void on_open_clicked();
    void on_previous_clicked();
    void on_play_clicked(bool checked);
    void on_mute_clicked(bool checked);
    void on_volume_valueChanged(int value);
    void on_stop_clicked();
    void on_next_clicked();
    void on_medialist_doubleClicked(const QModelIndex &index);
    void on_playlist_clicked(bool checked);
};

#endif // COREPLAYER_H
