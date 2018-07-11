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

#include "coreaction/coreaction.h"

#include <QApplication>
#include <QFont>
#include <QStyleFactory>

#include "corebox/globalfunctions.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_EnableHighDpiScaling);
    app.setQuitOnLastWindowClosed(false);

    QApplication::setStyle(QStyleFactory::create("Fusion"));

    // Set application info
    app.setOrganizationName("coreaction");
    app.setApplicationName("coreaction");
    app.setWindowIcon(QIcon(":/icons/CoreAction.svg"));

    coreaction a;
    a.show();

    // Get file folder size with file and folder count
//    int files = 0, folders = 0;
//    qDebug() << "File Size : " <<formatSize(getF(QStringList() << "/home/abrar", files, folders));
//    qDebug() << "File count : " << files;
//    qDebug() << "Folder count : " << folders;

// Folder paths to text file
//    QString out;
//    getFolderConts(out, "/home/abrar/Desktop/Builds");
//    QFile fi("/home/abrar/Desktop/HOO");
//    fi.open(QIODevice::WriteOnly | QIODevice::Text);
//    QTextStream t(&fi);
//    t << out;
//    fi.close();

// Delete last lines at a file
    //deleteLastLine("/home/abrar/.config/coreBox/RecentActivity1");

    return app.exec();
}
