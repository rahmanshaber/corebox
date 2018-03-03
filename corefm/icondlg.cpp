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

#include "icondlg.h"

#include <QtConcurrent/QtConcurrent>


icondlg::icondlg()
{
    setWindowTitle(tr("Select icon"));

    iconList = new QListWidget;
    iconList->setIconSize(QSize(24,24));
    iconList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    buttons = new QDialogButtonBox;
    buttons->setStandardButtons(QDialogButtonBox::Save|QDialogButtonBox::Cancel);
    connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(iconList);
    layout->addWidget(buttons);
    setLayout(layout);

    QSettings inherits("/usr/share/icons/" + QIcon::themeName() + "/index.theme",QSettings::IniFormat,this);
    foreach(QString theme, inherits.value("Icon Theme/Inherits").toStringList())
	themes.prepend(theme);
    themes.append(QIcon::themeName());

    thread.setFuture(QtConcurrent::run(this,&icondlg::scanTheme));
    connect(&thread,SIGNAL(finished()),this,SLOT(loadIcons()));
}

void icondlg::scanTheme()
{
    foreach(QString theme, themes)
    {
        QDirIterator it("/usr/share/icons/" + theme,QStringList("*.png"),QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks, QDirIterator::Subdirectories);
        while (it.hasNext())
        {
            it.next();
            fileNames.append(QFileInfo(it.fileName()).baseName());
        }
    }

    fileNames.removeDuplicates();
    fileNames.sort();
}

void icondlg::loadIcons()
{
    int counter = 0;

    foreach(QString name, fileNames)
    {
        new QListWidgetItem(QIcon::fromTheme(name),name,iconList);
        fileNames.removeOne(name);
        counter++;
        if(counter == 20)
        {
            QTimer::singleShot(50,this,SLOT(loadIcons()));
            return;
        }
    }
}

void icondlg::accept()
{
    result = iconList->currentItem()->text();
    this->done(1);
}
