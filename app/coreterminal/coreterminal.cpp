/*
CoreTerminal is free software; you can redistribute it and/or
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

#include <QApplication>
#include <QDebug>
#include <QHBoxLayout>
#include <QShortcut>
#include <QIcon>
#include <QProcessEnvironment>

coreterminal::coreterminal(QWidget *parent)
    : QWidget(parent)
{
    setWindowIcon(QIcon::fromTheme("utilities-terminal"));
    //QIcon::setThemeName("oxygen");

    QTermWidget *console = new QTermWidget();

    console->setStyleSheet("QScrollBar:vertical {"
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

    console->setTerminalOpacity(.95);
    console->setWindowOpacity(.95);

    /* Set the enivronment variable TERM as xterm */
    QProcessEnvironment procEnv = QProcessEnvironment::systemEnvironment();
    procEnv.insert("TERM", "xterm-256color");
    console->setEnvironment(QStringList() << "TERM=xterm-256color" << "COLORFGBG=15;0");
    console->setBlinkingCursor(true);

    /* Choose any of them for a default
     *
     * availableColorSchemes:
        "DarkPastels",
        "BreezeModified",
        "BlackOnLightYellow",
        "WhiteOnBlack",
        "SolarizedLight",
        "BlackOnWhite",
        "GreenOnBlack",
        "Solarized",
        "BlackOnRandomLight",
        "Linux"
        */
    console->setColorScheme("Linux");
    /* Choose any of them for a default
     *
     * availableKeyBindings:
     *  "linux",
     *  "macbook",
     *  "default",
     *  "vt420pc",
     *  "solaris"
     *
     */
    console->setKeyBindings("linux");

    console->setMotionAfterPasting(2);
    console->setFlowControlEnabled(true);
    console->setFlowControlWarningEnabled(true);

    console->setScrollBarPosition(QTermWidget::ScrollBarRight);

    //For Test a widget is created here.
    //You can do like qPDF
    QWidget *main = new QWidget(this);
    QHBoxLayout *hl = new QHBoxLayout();
    hl->setContentsMargins(0,0,0,0);
    hl->addWidget(console);

    main->resize(600,400);
    main->setLayout(hl);

    console->setFocusPolicy(Qt::StrongFocus);
    console->setFocus();
    //console->show();
    //Some shortcuts i collected from newbreeze if you want to add them, then keep them
    QShortcut *shortcut;

    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_F), this);
    connect(shortcut, &QShortcut::activated, console, &QTermWidget::toggleShowSearchBar);

    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_X), this);
    connect(shortcut, &QShortcut::activated, console, &QTermWidget::clear);

    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_C), this);
    connect(shortcut, &QShortcut::activated, console, &QTermWidget::copyClipboard);

    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_V), this);
    connect(shortcut, &QShortcut::activated, console, &QTermWidget::pasteClipboard);

    // real startup
    QObject::connect(console, SIGNAL(finished()), this, SLOT(close()));

    qDebug() << console->environment();
}

coreterminal::~coreterminal()
{

}
