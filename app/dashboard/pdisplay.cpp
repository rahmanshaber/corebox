#include "pdisplay.h"
#include "ui_pdisplay.h"

#include <QListView>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScreen>
#include <QStringListModel>

#include "corebox/globalfunctions.h"

pDisplay::pDisplay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::pDisplay)
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
    left << "Name        " << "Size        " << "Manufacturer        " << "Model      "
         << "Serial Number         " << "Refresh Rate        " << "Actual Resolution     "
         << "Set Resolution        " << "Physical Dots Per Inch "
         << "Physical Size        " << "Primary Orientation   ";

    for (int i = 0; i < qApp->screens().count(); i++) {

        QSize s = qApp->screens()[i]->size();
        QString size(tr("(%1,%2)").arg(s.width()).arg(s.height()));

        QSize a = qApp->screens()[i]->availableVirtualSize();
        QString AvailableVS(tr("(%1,%2)").arg(a.width()).arg(a.height()));

        QRect g = qApp->screens()[i]->geometry();
        QString Geometry(tr("(%1,%2)").arg(g.width()).arg(g.height()));

        QSizeF py = qApp->screens()[i]->physicalSize();
        QString PhysicalSize(tr("(%1,%2)").arg(py.width()).arg(py.height()));

        QStringList right;

        QListView *p = new QListView();
        QWidget *w = new QWidget();
        QFont fl ("Cantarell", 14, QFont::Normal);
        QFont fp ("Ubuntu Mono", 11, QFont::Normal);
        QLabel *l = new QLabel("Screen : " + QString::number(i+1));
        QVBoxLayout *v = new QVBoxLayout();
        QHBoxLayout *h = new QHBoxLayout();


        right << qApp->screens()[i]->name() << size << qApp->screens()[i]->manufacturer()
              << qApp->screens()[i]->model() << qApp->screens()[i]->serialNumber()
              << QString::number(qApp->screens()[i]->refreshRate()) << AvailableVS << Geometry
              << QString::number(qApp->screens()[i]->physicalDotsPerInch()) << PhysicalSize
              << QString::number(qApp->screens()[i]->primaryOrientation());

        QStringListModel *systemInfoModel = new QStringListModel(fStringList(left, right, p->font()));

        p->setModel(systemInfoModel);

        l->setFont(fl);
        p->setFont(fp);

        h->addWidget(p);
        h->setContentsMargins(1, 1, 1, 5);

        v->addWidget(l);
        v->addLayout(h);

        w->setLayout(v);

        p->setStyleSheet("QWidget{background-color: #1B252F; ;color: #ffffff;padding: 5px 5px 5px 5px;}");
        w->setStyleSheet("QWidget{background-color:#1F2B38;color:#ffffff;}");

        ui->list->addWidget(w);

        right.clear();
    }
}
