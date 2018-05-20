#include "corepdf.h"

#include <QFile>
#include <QVBoxLayout>
#include <QDebug>

#include "corebox/globalfunctions.h"

corepdf::corepdf(QWidget *parent):QWidget(parent)
{
//    openPdfFile("/home/shaber/Desktop/p.pdf");

     setStyleSheet("QWidget{background-color: #3E3E3E;border: 1px #2A2A2A;}");
}

corepdf::~corepdf()
{
    delete PdfWidget;
}

void corepdf::eclose(){
    QPdfWidget *cpdf = this->findChild<QPdfWidget*>("QPdfWidget");
    cpdf->close();
}

void corepdf::openPdfFile(const QString path)
{
    workFilePath = path;
    QVBoxLayout * mainLayout = new QVBoxLayout();
    PdfWidget = new QPdfWidget();

    connect(PdfWidget, &QPdfWidget::initialized, [this,path]() {
        PdfWidget->setToolbarVisible(false);
        QFile f(path);
        if (f.open(QIODevice::ReadOnly)) {
            QByteArray data = f.readAll();
            PdfWidget->loadData(data);
            f.close();
        }
    });

    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->addWidget(PdfWidget);
    setLayout(mainLayout);
}

void corepdf::closeEvent(QCloseEvent *event)
{
    event->ignore();
    qDebug()<< workFilePath;
    saveToRecent("CorePDF", workFilePath);
    event->accept();
}
