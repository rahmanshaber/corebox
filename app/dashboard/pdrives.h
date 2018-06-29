#ifndef PDRIVES_H
#define PDRIVES_H

#include <QWidget>
#include "corefm/udisks2.h"

namespace Ui {
class pDrives;
}

class pDrives : public QWidget
{
    Q_OBJECT

public:
    explicit pDrives(QWidget *parent = 0);
    ~pDrives();
    void reload();

private:
    Ui::pDrives *ui;
    UDisks2 *disks;

    QString getDriveInfo(const QString path);
    void setupDisksPage();

private slots:
    void on_drives_currentTextChanged(const QString &currentText);
    void on_blocks_currentTextChanged(const QString &currentText);
    void udisks2_blockAdded(const QString &node);
    void udisks2_blockRemoved(const QString &node);
    void udisks2_blockChanged(const QString &node);
    void udisks2_driveAdded(const QString &node);
    void udisks2_driveRemoved(const QString &node);
    void udisks2_driveChanged(const QString &node);
    void udisks2_filesystemAdded(const QString &node);
    void udisks2_filesystemRemoved(const QString &node);
    void udisks2_filesystemChanged(const QString &node);
    void on_mount_2_clicked();
    void on_unmount_2_clicked();
    void on_blocks_itemSelectionChanged();
    void on_drives_itemSelectionChanged();

};

#endif // PDRIVES_H
