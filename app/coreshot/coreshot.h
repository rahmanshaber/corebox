#ifndef CORESHOT_H
#define CORESHOT_H

#include <QWidget>

#include "corepaint/corepaint.h"
#include "../corebox/corebox.h"
#include "../settings/settingsmanage.h"
#include "../corebox/globalfunctions.h"

#include <QPixmap>

namespace Ui {
class coreshot;
}

class coreshot : public QWidget
{
    Q_OBJECT

public:
    explicit coreshot(QWidget *parent = 0);
    ~coreshot();

    void setPixmap(QPixmap &pix);

private slots:
    void on_openInCorePaint_clicked();

    void on_save_clicked();

    void on_saveAs_clicked();

    void on_cancel_clicked();

    void on_newShot_clicked();

private:
    Ui::coreshot *ui;

    SettingsManage sm;
    QString files;
};

#endif // CORESHOT_H
