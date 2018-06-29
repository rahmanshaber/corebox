/*
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

#include "coreterminal.h"
#include "../corebox/corebox.h"

#include <QDebug>
#include <QTabWidget>
#include <QShortcut>


coreterminal::coreterminal(QWidget *parent) : QTermWidget(0, parent)
{
    // Terminal Opacity
    setTerminalOpacity(.95);

    /* Set the enivronment variable TERM as xterm */
    QProcessEnvironment procEnv = QProcessEnvironment::systemEnvironment();
    procEnv.insert("TERM", "xterm-256color");
    setEnvironment(procEnv.toStringList());

    // Available Color Schemes
    /*  BlackOnLightYellow.schema
        BlackOnRandomLight.colorscheme
        BlackOnWhite.schema
        DarkPastels.colorscheme
        GreenOnBlack.colorscheme
        Linux.colorscheme
        Transparent.colorscheme
        WhiteOnBlack.schema
     */
    setColorScheme("Linux");
    setKeyBindings("linux");
    setScrollBarPosition(QTermWidget::ScrollBarRight);
    setStyleSheet("QScrollBar:vertical {"
                  "background-color: rgba( 0, 0, 0, 72% );"
                  "max-width: 8px;"
                  "border-radius: 4px;"
                  "padding: 1px; }"
                  "QScrollBar::handle:vertical {"
                  "background-color: silver;"
                  "max-width: 6px;"
                  "border-radius: 3px; }"
                  "QScrollBar::sub-page:vertical {"
                  "background-color: rgba( 0, 0, 0, 72% ); }"
                  "QScrollBar::add-page:vertical {"
                  "background-color: rgba( 0, 0, 0, 72% ); }"
                  "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
                  "width: 0px; height: 0px; }"
                  );

    // Set font
    //setTerminalFont();

    //How many lines going to be in one window.
    setHistorySize(100000);

    setShellProgram("/bin/bash");

    setMotionAfterPasting(2);
    setFlowControlEnabled(true);
    setFlowControlWarningEnabled(true);

    startShellProgram();
    watcher->addPath(QString("/proc/%1/").arg(getShellPID()));
    oldCWD = QFileInfo(QString("/proc/%1/cwd").arg(getShellPID())).symLinkTarget();
    connect(watcher, SIGNAL(directoryChanged(const QString&)), this, SLOT(handleFSWSignals(QString)));

    setFocus();
}

coreterminal::coreterminal(const QString &workDir, QWidget *parent) : QTermWidget(0, parent)
{
    // Terminal Opacity
    setTerminalOpacity(.95);

    /* Set the enivronment variable TERM as xterm */
    QProcessEnvironment procEnv = QProcessEnvironment::systemEnvironment();
    procEnv.insert("TERM", "xterm-256color");
    setEnvironment(procEnv.toStringList());

    // Available Color Schemes
    /*  BlackOnLightYellow.schema
        BlackOnRandomLight.colorscheme
        BlackOnWhite.schema
        DarkPastels.colorscheme
        GreenOnBlack.colorscheme
        Linux.colorscheme
        Transparent.colorscheme
        WhiteOnBlack.schema
     */
    setColorScheme("Linux");
    setScrollBarPosition(QTermWidget::ScrollBarRight);
    setStyleSheet("QScrollBar:vertical {"
                  "background-color: rgba( 0, 0, 0, 72% );"
                  "max-width: 8px;"
                  "border-radius: 4px;"
                  "padding: 1px; }"
                  "QScrollBar::handle:vertical {"
                  "background-color: silver;"
                  "max-width: 6px;"
                  "border-radius: 3px; }"
                  "QScrollBar::sub-page:vertical {"
                  "background-color: rgba( 0, 0, 0, 72% ); }"
                  "QScrollBar::add-page:vertical {"
                  "background-color: rgba( 0, 0, 0, 72% ); }"
                  "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
                  "width: 0px; height: 0px; }"
                  );

    // Set font
    //setTerminalFont();

    //How many lines going to be in one window.
    setHistorySize(100000);

    setWorkingDirectory(workDir);

    setShellProgram("/bin/bash");

    setMotionAfterPasting(2);
    setFlowControlEnabled(true);
    setFlowControlWarningEnabled(true);

    startShellProgram();

    setFocus();

}

coreterminal::coreterminal(const QString &workDir, const QString &command, QWidget *parent) : QTermWidget(0, parent)
{
    // Terminal Opacity
    setTerminalOpacity(.95);
    setWindowOpacity(.95);

    /* Set the enivronment variable TERM as xterm */
    QProcessEnvironment procEnv = QProcessEnvironment::systemEnvironment();
    procEnv.insert("TERM", "xterm-256color");
    setEnvironment(procEnv.toStringList());

    // Available Color Schemes
    /*  BlackOnLightYellow.schema
        BlackOnRandomLight.colorscheme
        BlackOnWhite.schema
        DarkPastels.colorscheme
        GreenOnBlack.colorscheme
        Linux.colorscheme
        Transparent.colorscheme
        WhiteOnBlack.schema
     */
    setColorScheme("Linux");
    setKeyBindings("linux");
    setScrollBarPosition(QTermWidget::ScrollBarRight);
    setStyleSheet("QScrollBar:vertical {"
                  "background-color: rgba( 0, 0, 0, 72% );"
                  "max-width: 8px;"
                  "border-radius: 4px;"
                  "padding: 1px; }"
                  "QScrollBar::handle:vertical {"
                  "background-color: silver;"
                  "max-width: 6px;"
                  "border-radius: 3px; }"
                  "QScrollBar::sub-page:vertical {"
                  "background-color: rgba( 0, 0, 0, 72% ); }"
                  "QScrollBar::add-page:vertical {"
                  "background-color: rgba( 0, 0, 0, 72% ); }"
                  "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
                  "width: 0px; height: 0px; }"
                  );

    // Set font
    //setTerminalFont();

    //How many lines going to be in one window.
    setHistorySize(100000);

    setWorkingDirectory(workDir);
    qDebug() << workDir << "\nOPOP";
    QStringList shArgs = QStringList() << "-il" << "-c" << command;			//.split( QRegExp( "\\s+" ), QString::SkipEmptyParts );

    setShellProgram("/bin/bash");
    if (!command.isEmpty())
        setArgs(shArgs);

    setMotionAfterPasting(2);
    setFlowControlEnabled(true);
    setFlowControlWarningEnabled(true);

    startShellProgram();

    setFocus();

    connect(this, &coreterminal::finished, [this]() {
        CoreBox *cBox = static_cast<CoreBox*>(qApp->activeWindow());
        cBox->closeCurrentTab();
    });

    QShortcut *shortcut;

    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_C), this);
    connect(shortcut, &QShortcut::activated, this, &coreterminal::copyClipboard);
    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_V), this);
    connect(shortcut, &QShortcut::activated, this, &coreterminal::pasteClipboard);
}

QString coreterminal::currentWorkingDirectory()
{
    QString cwd = QString("/proc/%1/cwd").arg(getShellPID());
    return QFileInfo(cwd).symLinkTarget();
}

void coreterminal::handleFSWSignals(QString)
{
    if (QFileInfo(QString("/proc/%1/cwd").arg(getShellPID())).symLinkTarget() == oldCWD)
        return;

    oldCWD = QFileInfo(QString("/proc/%1/cwd").arg(getShellPID())).symLinkTarget();
    emit chDir(oldCWD);
}

void coreterminal::closeEvent(QCloseEvent *cEvent)
{
    cEvent->ignore();
    emit finished();
    sendText("exit\n");
    cEvent->accept();
}

