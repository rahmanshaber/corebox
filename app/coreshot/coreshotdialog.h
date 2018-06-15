#ifndef CORESHOTDIALOG_H
#define CORESHOTDIALOG_H

#include <QWidget>
#include <QPixmap>

#include "coreshot/screenwidget.h"
#include "coreshot/modeselectarea.h"
#include "coreshot/modefullscreen.h"
#include "coreshot/previewwidget.h"


class CoreShotDialog : public QWidget
{
    Q_OBJECT
public:
    explicit CoreShotDialog(QWidget *parent = nullptr);

private:
    QPixmap m_pixmap;

    WId getActiveWindowId();
    QRect getWindowFrame(WId wid);

    void passToShotPreview();
    ModeSelectArea *selcArea;
    ModeFullscreen *fullSc;

private slots:
    void shootActiveWindow();
    void shootFullScreen();
    void shootSelectArea();

};

#endif // CORESHOTDIALOG_H
