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

#include "coreplayer.h"
#include "ui_coreplayer.h"

#include <QMediaService>
#include <QVideoProbe>
#include <QAudioProbe>
#include <QMediaMetaData>
#include <QtWidgets>
#include <QVideoWidget>
#include <QStandardItemModel>


coreplayer::coreplayer(QWidget *parent):QWidget(parent),ui(new Ui::coreplayer)
   , playerState(QMediaPlayer::StoppedState)
{
    qDebug() << "coreplayer opening";
    ui->setupUi(this);

    mModel = new QStandardItemModel(this);
    mNumberOfFiles = 0;
    ui->shortcut->setVisible(0);
    ui->folderLineEdit->setReadOnly(true);
    ui->medialist->setModel(mModel);

    player = new QMediaPlayer(this);
    connect(player, SIGNAL(durationChanged(qint64)), SLOT(durationChanged(qint64)));
    connect(player, SIGNAL(positionChanged(qint64)), SLOT(positionChanged(qint64)));
    connect(player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),this, SLOT(statusChanged(QMediaPlayer::MediaStatus)));
    connect(player, SIGNAL(bufferStatusChanged(int)), this, SLOT(bufferingProgress(int)));
    connect(player, SIGNAL(videoAvailableChanged(bool)), this, SLOT(videoAvailableChanged(bool)));
    connect(player, SIGNAL(error(QMediaPlayer::Error)), this, SLOT(displayErrorMessage()));
//    connect(player, &QMediaPlayer::stateChanged, this, &coreplayer::stateChanged);

    QVideoWidget *Vwidget = new QVideoWidget(this);
    player->setVideoOutput(Vwidget);
    ui->view->addWidget(Vwidget);

    if(isPlayerAvailable()){
        for (QPushButton *b : ui->navigation->findChildren<QPushButton*>()){
            b->setEnabled(false);
        }
        ui->playlist->setEnabled(true);
        ui->seekBar->setEnabled(false);
        ui->volume->setEnabled(false);
    }

    ui->seekBar->setRange(0, player->duration() / 1000);
    connect(ui->seekBar,SIGNAL(sliderMoved(int)),this,SLOT(seek(int)));

    if (!isPlayerAvailable()) {
        messageEngine(tr("The QMediaPlayer object does not have a valid service\nPlease check the media service plugins are installed."),"Warning");
     }

    ui->numberOfFiles->setVisible(0);

    audioMimes();
    videoMimes();
    shotcuts();
}

coreplayer::~coreplayer()
{
    qDebug()<<"corepalyer closing";
    delete ui;
}

void coreplayer::shotcuts(){
    QShortcut* shortcut;

    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_O), this);
    connect(shortcut, &QShortcut::activated, this, &coreplayer::on_open_clicked);
    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Left), this);
    connect(shortcut, &QShortcut::activated, this, &coreplayer::on_next_clicked);
    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Right), this);
    connect(shortcut, &QShortcut::activated, this, &coreplayer::on_previous_clicked);
    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Enter), this);
    connect(shortcut, &QShortcut::activated, this, &coreplayer::on_stop_clicked);
//    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Space), this);
//    connect(shortcut, &QShortcut::activated, this, &coreplayer::on_play_clicked);
//    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_M), this);
//    connect(shortcut, &QShortcut::activated, this, &coreplayer::on_mute_clicked);

    shortcut = new QShortcut(QKeySequence(Qt::Key_Left), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(seekLeft()));
    shortcut = new QShortcut(QKeySequence(Qt::Key_Right), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(seekRight()));
    shortcut = new QShortcut(QKeySequence(Qt::Key_Up), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(volumnUP()));
    shortcut = new QShortcut(QKeySequence(Qt::Key_Down), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(volumnDown()));
    //seekbar seeking(4s) by right left arrow
    //volumn up down by up down arrow
}

void coreplayer::closeEvent(QCloseEvent *event) {
    Q_UNUSED(event);
    on_stop_clicked();
}

void coreplayer::videoMimes() {

    QMimeDatabase mime;
    QList<QMimeType> d = mime.allMimeTypes();

    foreach (QMimeType s, d) {
        //Store for Video
        if (s.name().startsWith("video"))
            if (!s.preferredSuffix().isNull())
                videomimes.append(s.name());
    }

//    audiomimes.replace(audiomimes.indexOf("audio/mpeg"), "audio/mp3");//error
    videomimes.sort();
}

QStringList coreplayer::getVideos(const QString path) {

    QDir dir(path);
    QStringList videos;
    for (QString file : dir.entryList()) {
        if (file == "." || file == "..") {
            continue;
        }
            //for each video check the file.
            foreach (QString s, videomimes) {
                if (QFileInfo(file).completeSuffix() == s.split("/").at(1) || QFileInfo(file).suffix() == s.split("/").at(1))
                    videos.append(path + "/" + file);
            }
    }
    return videos;
}

void coreplayer::audioMimes() {

    QMimeDatabase mime;
    QList<QMimeType> d = mime.allMimeTypes();

    foreach (QMimeType s, d) {
        //Store for Audio
        if (s.name().startsWith("audio"))
            if (!s.preferredSuffix().isNull())
                audiomimes.append(s.name());
    }

    audiomimes.replace(audiomimes.indexOf("audio/mpeg"), "audio/mp3");
    audiomimes.sort();

}

QStringList coreplayer::getAudios(const QString path) {

    QDir dir(path);
    QStringList audios;
    for (QString file : dir.entryList()) {
        if (file == "." || file == "..") {
            continue;
        }
            //for each audio check the file.
            foreach (QString s, audiomimes) {
                if ((QFileInfo(file).completeSuffix()) == (s.split("/").at(1)) || QFileInfo(file).suffix() == s.split("/").at(1))
                    audios.append(path + "/" + file);
            }
    }
    return audios;
}

bool coreplayer::isPlayerAvailable() const
{
    return player->isAvailable();
}

void coreplayer::durationChanged(qint64 duration)
{
    this->duration = duration/1000;
    ui->seekBar->setMaximum(duration / 1000);
}

void coreplayer::positionChanged(qint64 progress)
{
    if (!ui->seekBar->isSliderDown()) {
        ui->seekBar->setValue(progress / 1000);
    }
    updateDurationInfo(progress / 1000);
}

void coreplayer::seek(int seconds)
{
    player->setPosition(seconds * 1000);
}

void coreplayer::statusChanged(QMediaPlayer::MediaStatus status)
{
    handleCursor(status);

    // handle status message
    switch (status) {
    case QMediaPlayer::UnknownMediaStatus:
    case QMediaPlayer::NoMedia:
    case QMediaPlayer::LoadedMedia:
    case QMediaPlayer::BufferingMedia:
    case QMediaPlayer::BufferedMedia:
//        setStatusInfo(QString());
        break;
    case QMediaPlayer::LoadingMedia:
        messageEngine("Loading...", "Info");
        break;
    case QMediaPlayer::StalledMedia:
        messageEngine("Media Stalled", "Info");
        break;
    case QMediaPlayer::EndOfMedia:
        QApplication::alert(this);
        break;
    case QMediaPlayer::InvalidMedia:
        messageEngine("InvalidMedia", "Warning");
        break;
    }
}

void coreplayer::handleCursor(QMediaPlayer::MediaStatus status)
{
#ifndef QT_NO_CURSOR
    if (status == QMediaPlayer::LoadingMedia ||
        status == QMediaPlayer::BufferingMedia ||
        status == QMediaPlayer::StalledMedia)
        setCursor(QCursor(Qt::BusyCursor));
    else
        unsetCursor();
#endif
}

void coreplayer::bufferingProgress(int progress)
{
    messageEngine(tr("Buffering %4%").arg(progress), "Info");
}

void coreplayer::displayErrorMessage()
{
    messageEngine(player->errorString(), "Warning");
}

void coreplayer::updateDurationInfo(qint64 currentInfo)
{
    QString time;
    if (currentInfo || duration) {
        QTime currentTime((currentInfo/3600)%60, (currentInfo/60)%60, currentInfo%60, (currentInfo*1000)%1000);
        QTime totalTime((duration/3600)%60, (duration/60)%60, duration%60, (duration*1000)%1000);
        QString format = "mm:ss";
        if (duration > 3600)
            format = "hh:mm:ss";
        time = currentTime.toString(format) + " / " + totalTime.toString(format);
    }
    ui->duration->setText(time);
}

void coreplayer::creatPlayList(const QString path)
{
    QFileInfo sp(path);
    QString selectedFilePath;
    QModelIndex index = mModel->index(0, 0);
    sp.isDir() ? selectedFilePath = path : selectedFilePath = sp.path();
    mModel->clear();
    ui->folderLineEdit->setText(selectedFilePath);
    QStringList list1 = getAudios(selectedFilePath);
    QStringList list2 = getVideos(selectedFilePath);
    QStringList lists;
    lists.append(list1);
    lists.append(list2);
    list1.clear();
    list2.clear();
    lists = lists.toSet().toList();
    for (int i = 0; i < lists.count(); ++i) {
        mModel->appendRow((new QStandardItem(QFileInfo(lists.at(i)).fileName())));
        if (sp.isFile() && (sp.fileName() == mModel->index(i, 0).data().toString())) {
            index = mModel->index(i, 0);
        }
    }
    mModel->sort(0);
    mNumberOfFiles = lists.count();
    if(mNumberOfFiles > 0){
         ui->numberOfFiles->setVisible(1);
         ui->numberOfFiles->setText(QString("Included Files: %1").arg(mNumberOfFiles));
    }
}

void coreplayer::openPlayer(const QString path)
{
    //send path to creat playlist.
    creatPlayList(path);

    if(!path.isEmpty()){
        for (QPushButton *b : ui->navigation->findChildren<QPushButton*>()){
            b->setEnabled(true);
        }
        ui->seekBar->setEnabled(true);
        ui->volume->setEnabled(true);
        player->setMedia(QUrl::fromLocalFile(path));
        player->play();
        ui->play->setChecked(true);
        messageEngine("Playing", "Info");
    }

}

void coreplayer::on_open_clicked()
{
    QFileDialog dialog(this, tr("Open Media File"));
    QStringList mimes;
    mimes.append(audiomimes);
    mimes.append(videomimes);
    dialog.setMimeTypeFilters(mimes);
    dialog.selectMimeTypeFilter("audio/mp3");
    if (dialog.exec() == QDialog::Accepted) { //if dialog pressed accept means open then do this.
//        openPlayer(QFileInfo(dialog.selectedFiles().first()).path());
        filepath = dialog.selectedFiles().first();
        folderpath = QFileInfo(dialog.selectedFiles().first()).path();
        openPlayer(filepath);
        messageEngine("Files Collected", "Info");
        for (QPushButton *b : ui->navigation->findChildren<QPushButton*>()){
            b->setEnabled(true);
        }
        ui->seekBar->setEnabled(true);
        ui->volume->setEnabled(true);
    } else {
        messageEngine("Files collection rejected", "Info");
    }
}

QMediaPlayer::State coreplayer::state() const
{
    return playerState;
}

void coreplayer::setState(QMediaPlayer::State state)
{
    if (state != playerState) {
        playerState = state;

        switch (state) {
        case QMediaPlayer::StoppedState:
            ui->play->setChecked(0);
            break;
        case QMediaPlayer::PlayingState:
            ui->stop->setChecked(0);
            break;
        case QMediaPlayer::PausedState:
            ui->stop->setChecked(0);
            break;
        }
    }
}

void coreplayer::on_play_clicked(bool checked)
{
    if (checked ){
        setState(QMediaPlayer::PlayingState);
        player->play();
        if(ui->medialist->currentIndex().row() != -1){play(ui->medialist->currentIndex().row());}
        else{play(0);}
        if(ui->shortcut->isVisible()){ui->shortcut->setVisible(false);}
        messageEngine("Playing", "Info");
    }
    else{
        setState(QMediaPlayer::PausedState);
        player->pause();
        messageEngine("Paused", "Info");
    }
}

void coreplayer::on_mute_clicked(bool checked)
{
    if (checked){
        player->setMuted(true);
        messageEngine("Mute", "Info");
    }
    else{
        player->setMuted(false);
    }
}

void coreplayer::on_volume_valueChanged(int value)
{
   player->setVolume(value);
   QString vol = QString::number(value);
   ui->volume->setValue(value);
}

void coreplayer::on_stop_clicked()
{
    player->stop();
    ui->play->setChecked(false);
    player->setPosition(0);
    player->setMedia(NULL);
    ui->duration->setText("00:00 / 00:00");
    ui->workingOn->setText("");
}

void coreplayer::on_next_clicked()
{
    on_stop_clicked();
    if(ui->medialist->model()->rowCount() != ui->medialist->currentIndex().row()){
        play(ui->medialist->currentIndex().row() + 1);
    }
}

void coreplayer::on_previous_clicked()
{
    on_stop_clicked();
    if(ui->medialist->currentIndex().row() > -1){
        play(ui->medialist->currentIndex().row() - 1);
    }
}

void coreplayer::setCurrentIndex(int currentIndex)
{
    ui->medialist->setCurrentIndex(mModel->index(currentIndex, 0));
}

void coreplayer::setFolder(const QString &foldername)
{
    mModel->clear();
    ui->folderLineEdit->setText(foldername);
    QStringList list1 = getAudios(foldername);
    QStringList list2 = getVideos(foldername);

    for (int i = 0; i < list1.count(); ++i) {
        mModel->appendRow((new QStandardItem(QFileInfo(list1.at(i)).fileName())));
    }

    for (int i = 0; i < list2.count(); ++i) {
        mModel->appendRow((new QStandardItem(QFileInfo(list2.at(i)).fileName())));
    }

    mNumberOfFiles = list1.count() + list2.count();
    ui->numberOfFiles->setText(QString("Included Files: %1")
                                    .arg(mNumberOfFiles));
}

void coreplayer::play(int index)
{
    auto mediaPlayerState = player->state();
    if (mediaPlayerState == QMediaPlayer::PausedState) {
        player->play();
        return;
    } else if (mediaPlayerState == QMediaPlayer::PlayingState) {
        return;
    }
    if (index == -1 || index >= mNumberOfFiles) {
        return;
    }
    auto filename = ui->folderLineEdit->text()
            + "/" + mModel->index(index, 0).data().toString();
    player->setMedia(QUrl::fromLocalFile(filename));
    player->play();
    ui->medialist->setCurrentIndex(mModel->index(index, 0));
    ui->medialist->currentIndex().data().toInt();
    ui->workingOn->setText(mModel->index(index, 0).data().toString());
}

void coreplayer::on_medialist_doubleClicked(const QModelIndex &index)
{
    on_stop_clicked();
    play(index.row());
    ui->play->setChecked(true);
    messageEngine("Playing", "Info");
}

void coreplayer::seekLeft() {
    if (!(ui->seekBar->value() == 0))
    {seek(ui->seekBar->value() - 4);}
}

void coreplayer::seekRight() {
    if (!(ui->seekBar->value() == ui->seekBar->maximum()))
    {seek(ui->seekBar->value() + 4);}
}

void coreplayer::volumnDown() {
    int x = ui->volume->value() - 5;
    if (!(ui->volume->value() == 0))
    {on_volume_valueChanged(x);}
}

void coreplayer::volumnUP() {
    int x = ui->volume->value() + 5;
    if (!(x == ui->volume->maximum()))
    { on_volume_valueChanged(x); }
}

void coreplayer::on_playlist_clicked(bool checked)
{
    if(checked){
        ui->shortcut->setVisible(1);
    }else{
        ui->shortcut->setVisible(0);
    }
}
