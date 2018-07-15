#ifndef SESSIONSAVEDIALOG_H
#define SESSIONSAVEDIALOG_H

#include <QDialog>

namespace Ui {
class sessionSaveDialog;
}

class sessionSaveDialog : public QDialog
{
    Q_OBJECT

public:
    explicit sessionSaveDialog(QWidget *parent = 0);
    ~sessionSaveDialog();
    QString sName;

signals:
    void nameOk();

private slots:
    void on_ok_clicked();

    void on_sessionName_textChanged(const QString &arg1);

private:
    Ui::sessionSaveDialog *ui;
    QStringList nameList;
};

#endif // SESSIONSAVEDIALOG_H
