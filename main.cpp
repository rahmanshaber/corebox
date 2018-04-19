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

    QStringList result;
    const auto allMounted = QStorageInfo::mountedVolumes();
    result.reserve(allMounted.size());
    for(auto& singleMounted : allMounted)
    result << singleMounted.displayName();

//    const auto allMounted = QStorageInfo::mountedVolumes();
    for(int i=0; i<result.count(); ++i ){
        QString l;
        l = result.at(i);
        QStorageInfo(l).rootPath();
        qDebug()<<QStorageInfo(l).rootPath();
    }
//    result.reserve(allMounted.size());
//    for(auto& singleMounted : allMounted)
//    result << singleMounted.displayName();

    qDebug()<< result;

    coreaction a;
    a.show();

    return app.exec();
}
