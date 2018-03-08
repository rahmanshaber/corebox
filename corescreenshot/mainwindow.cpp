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

#include "mainwindow.h"

#include <QFileDialog>
#include <QClipboard>
#include <QSpinBox>


MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    qDebug() << "corescreenshot opening";
    cPreview  = new PreviewWidget();
    cShot     = new QPushButton("Take a new screenshot");
    cSave     = new QPushButton("Save");
    cCancel   = new QPushButton("Cancel");
    cCorePaint= new QPushButton("Send to CorePaint");
    cSaveAs   = new QPushButton("Save_As");
    cType     = new QComboBox();
    cdelaySec = new QSpinBox();
    cdelaylbl = new QLabel("Delay (sec) : ");
    cLable    = new QLabel("Capture Type:");

    addSnapMode(new ModeSelectArea());
    addSnapMode(new ModeFullscreen());

    QHBoxLayout * HLayout1 = new QHBoxLayout();
    QHBoxLayout * HLayout2  = new QHBoxLayout();
    QVBoxLayout * mainLayout = new QVBoxLayout();

    cPreview->setFrameShape(QFrame::Box	);
    cPreview->setFrameShadow(QFrame::Sunken);
    cPreview->setMinimumSize(528,297);                                   // screen ratio = 16:9
    cType->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    cShot->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Minimum);

    HLayout1->addWidget(cLable);
    HLayout1->addWidget(cType);
    HLayout1->addStretch();
    HLayout1->addWidget(cdelaylbl);
    HLayout1->addWidget(cdelaySec);
    HLayout1->addStretch();
    HLayout1->addWidget(cShot);

    HLayout2->addStretch();
    HLayout2->addWidget(cCorePaint);
    HLayout2->addWidget(cSave);
    HLayout2->addWidget(cSaveAs);
    HLayout2->addWidget(cCancel);

    mainLayout->addWidget(cPreview);
    mainLayout->addStretch();
    mainLayout->addLayout(HLayout1);
    mainLayout->addLayout(HLayout2);

    setLayout(mainLayout);
    cSave->setVisible(false);
    cSaveAs->setVisible(false);
    cCorePaint->setVisible(false);

    files = "";

    connect(cType, SIGNAL(currentIndexChanged(int)), this, SLOT(on_cType_IndexChanged(int)));
    connect(cShot,SIGNAL(clicked()),this,SLOT(waitAndTakeScreen()));
    connect(cCorePaint, SIGNAL(clicked()), this, SLOT(toCorePaint()));
    connect(cSave,SIGNAL(clicked()),this,SLOT(save()));
    connect(cSaveAs,SIGNAL(clicked()),this,SLOT(saveAs()));
    connect(cCancel,SIGNAL(clicked()),this,SLOT(close()));

    on_cType_IndexChanged(0);
    this->setWindowIcon(QIcon(":/icons/CoreShot.svg"));
    this->setWindowTitle("CoreShot");
//    setStyleSheet("QWidget{background-color: #2A2A2A;color: #ffffff;border: 1px #2A2A2A;padding: 5px 5px 5px 5px;}"););
}

MainWindow::~MainWindow()
{
    qDebug()<<"coreshot closing";

    delete cPreview;
    delete cShot;
    delete cSave;
    delete cSaveAs;
    delete cCancel;
    delete cType;
    delete cLable;
}

void MainWindow::waitAndTakeScreen()
{
    this->setWindowState(Qt::WindowMinimized);
    QTimer::singleShot(200,this,SLOT(takeScreen()));
}

void MainWindow::takeScreen()
{
    int index = cType->currentIndex();
    ScreenWidget * snapWidget = cSnapWidgets.at(index);
    if (index == 0) {
        snapWidget->showFullScreen();
        snapWidget->takeScreen();
    }
    if (index == 1) {
        delaying(cdelaySec->value());
    }
}

void MainWindow::delaying(int secs)
{
    QTimer::singleShot(secs * 1000, this, SLOT(emitToS()));
}

void MainWindow::emitToS()
{
    ScreenWidget * snapWidget = cSnapWidgets.at(cType->currentIndex());
    snapWidget->takeScreen();
    emit snapWidget->subscreenTaken();
}

void MainWindow::save()
{
    QString fileName = sm.getSCSaveLocation();
    QFile file(fileName);
    cPreview->originalPixmap().save(&file, "PNG");
    file.close();
    files = fileName;
    messageEngine("Screenshot Saved", "Info");
    this->close();
}

void MainWindow::saveAs()
{
    QString filename = QFileDialog::getSaveFileName(this,tr("Save Screenshot"),sm.getSCSaveLocation(), tr("Image (*.png)"));
    QFile file(filename);
    cPreview->originalPixmap().save(&file,"PNG");
    file.close();
    files = filename;
    messageEngine("Screenshot Saved", "Info");
    this->close();
}

void MainWindow::addSnapMode(ScreenWidget *widget)
{
    cSnapWidgets.append(widget);
    cType->addItem(widget->objectName());
    connect(widget,SIGNAL(subscreenTaken()),this,SLOT(setPreview()));
}

void MainWindow::toCorePaint()
{
    QString fileName = sm.getSCSaveLocation() + "/Screenshot_" + QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss") + ".png";
    QFile file(fileName);
    cPreview->originalPixmap().save(&file, "PNG");
    file.close();
    files = fileName;
    CoreBox *cBox = new CoreBox();
    cBox->tabEngine(3, files);
    this->close();
    cBox->show();
}

void MainWindow::setPreview()
{
    ScreenWidget * snapWidget = qobject_cast<ScreenWidget*>(sender());
    cPreview->setOriginalPixmap(snapWidget->subscreen());
    cShot->setFocus();

    cSave->setVisible(true);
    cSaveAs->setVisible(true);
    cCorePaint->setVisible(true);
    this->setWindowState(Qt::WindowNoState);
}

void MainWindow::on_cType_IndexChanged(int index) {
    if (index == 0) {
        cdelaylbl->setVisible(false);
        cdelaySec->setVisible(false);
    } else if (index == 1){
        cdelaylbl->setVisible(true);
        cdelaySec->setVisible(true);
    }
}
