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

#include "pdisplay.h"
#include "ui_pdisplay.h"


pDisplay::pDisplay(QWidget *parent) :QWidget(parent),ui(new Ui::pDisplay)
{
    ui->setupUi(this);
    setupDisplayPage();
}

pDisplay::~pDisplay()
{
    delete ui;
}

void pDisplay::setupDisplayPage()
{
    QStringList left;
    left << "Name" << "Size " << "Manufacturer" << "Model"
         << "Serial Number" << "Refresh Rate" << "Default Resolution"
         << "Set Resolution" << "Physical Dots Per Inch "
         << "Physical Size" <<  "Screen Size :" << "Default Orientation"
         << "Set Orientation"  ;

    for (int i = 0; i < qApp->screens().count(); i++) {

        QSize s = qApp->screens()[i]->size();
        QString size(tr("(%1,%2)").arg(s.width()).arg(s.height()));

        QSize a = qApp->screens()[i]->availableVirtualSize();
        QString availableVS(tr("(%1,%2)").arg(a.width()).arg(a.height()));

        QRect g = qApp->screens()[i]->geometry();
        QString geometry(tr("(%1,%2)").arg(g.width()).arg(g.height()));

        QSizeF py = qApp->screens()[i]->physicalSize();
        QString physicalSize(tr("(%1,%2,%3)").arg(py.width()).arg(py.height()).arg(" millimetre"));

        // screen size in inches
        int screenSize(qSqrt(qPow (py.width(),2) + qPow (py.height(),2)) * 0.03937008);


        // screen orientation
        QString defultOrientation = "";
        if ( qApp->screens()[i]->nativeOrientation() == Qt::LandscapeOrientation ){
            defultOrientation = "Landscape";
        }else{
            defultOrientation = "Portrait";
        }

        QString setOrientation = "";
        if ( qApp->screens()[i]->primaryOrientation() == Qt::LandscapeOrientation ){
            setOrientation = "Landscape";
        }else{
            setOrientation = "Portrait";
        }

        QStringList right;

        QListView *p = new QListView();
        p->setFocusPolicy(Qt::NoFocus);
        QWidget *w = new QWidget();
        w->setMinimumHeight(300);
        w->setAttribute(Qt::WA_TransparentForMouseEvents);
        w->setFocusPolicy(Qt::NoFocus);
        QString title = ("Screen : " + QString::number(i+1));
        QGroupBox *b = new QGroupBox(tr("title"));
        QVBoxLayout *v = new QVBoxLayout();
        QHBoxLayout *h = new QHBoxLayout();


        right << qApp->screens()[i]->name() << size << qApp->screens()[i]->manufacturer()
              << qApp->screens()[i]->model() << qApp->screens()[i]->serialNumber()
              << QString::number(qApp->screens()[i]->refreshRate()) + " Hz" << availableVS << geometry
              << QString::number(qApp->screens()[i]->physicalDotsPerInch()) << physicalSize
              << QString::number(screenSize) + " inches"
              << defultOrientation << setOrientation;


        QStringListModel *displayInfo = new QStringListModel(fStringList(left, right, p->font()));

        p->setModel(displayInfo);

        h->addWidget(p);
        b->setTitle(title);
        h->setContentsMargins(8, 25, 8, 10);
        b->setLayout(h);

        v->addWidget(b);
        w->setLayout(v);

        addDropShadow(w, 40);

        ui->list->addWidget(w);

        right.clear();
    }

    QSpacerItem *verticalSpacer = new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);
    ui->list->addItem(verticalSpacer);
}
