#ifndef COREARCHIVER_H
#define COREARCHIVER_H

#include <QWidget>
#include <QFileDialog>

#include "libarchiveqt.h"

namespace Ui {
class corearchiver;
}

class corearchiver : public QWidget
{
    Q_OBJECT

public:
    explicit corearchiver(QWidget *parent = 0);
    ~corearchiver();

    void compress(QStringList filePathList, QDir currentDir);
    void extract(QString archiveFilePath, QDir dest);

    QString archiveName, location, format, workingDir;
    QStringList filePathList;

private Q_SLOTS:
    void updateFileName( QString );
    void updateDirName();
    void updateFormat( QString );

private:
    Ui::corearchiver *ui;
};

#endif // COREARCHIVER_H
