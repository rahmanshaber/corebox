#ifndef RENAMEWINDOW_H
#define RENAMEWINDOW_H

#include <QWidget>
#include <QFileInfo>

namespace Ui {
class RenameWindow;
}

class RenameWindow : public QWidget
{
    Q_OBJECT

public:
    explicit RenameWindow(QFileInfo iFile, QPixmap pix, QWidget *parent = 0);
    ~RenameWindow();

private slots:
    void on_ok_clicked();

private:
    Ui::RenameWindow *ui;
    QFileInfo m_iFile;
};

#endif // RENAMEWINDOW_H
