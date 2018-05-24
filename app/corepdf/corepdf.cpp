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

#include "corepdf.h"

#include <QFile>
#include <QVBoxLayout>
#include <QDebug>
#include <QPushButton>

#include "corebox/globalfunctions.h"

corepdf::corepdf(QWidget *parent):QWidget(parent)
{

//    setObjectName("corepdf");

    QVBoxLayout * mainLayout = new QVBoxLayout();
    PdfWidget = new QPdfWidget();
    setObjectName("corepdf");
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->addWidget(PdfWidget);
//    cShot     = new QPushButton("close");
//    connect(cShot,SIGNAL(clicked()),this,SLOT(eclose()));
//    mainLayout->addWidget(cShot);
    setLayout(mainLayout);
}

corepdf::~corepdf()
{
    delete PdfWidget;
}

void corepdf::eclose()
{
    QPdfWidget *cpdf = this->findChild<QPdfWidget*>("QPdfWidget");
        if (cpdf != nullptr) {
            cpdf->closeDocument();
        } else {
            qWarning() << "Not found!";
        }
}

void corepdf::openPdfFile(const QString path)
{
    workFilePath = path;

//    connect(PdfWidget, &QPdfWidget::initialized, [this,path]() {
//        PdfWidget->setToolbarVisible(false);
        QFile f(path);
        if (f.open(QIODevice::ReadOnly)) {
            QByteArray data = f.readAll();
            PdfWidget->loadData(data);
            f.close();
        }
//    });

}

void corepdf::closeEvent(QCloseEvent *event)
{
    event->ignore();
    saveToRecent("CorePDF", workFilePath);
    event->accept();
}
