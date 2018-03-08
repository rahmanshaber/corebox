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

#include "bookmarkdialog.h"

#include <QDebug>


bookmarkDialog::bookmarkDialog(QWidget *parent)
    :QDialog(parent)
    ,accept(new QPushButton("Done"))
    ,cancel(new QPushButton("Cancel"))
    ,statusLabel(new QLabel)
    ,lSection(new QLabel("Section Name : "))
    ,lbkName(new QLabel("Bookmark Name : "))
    ,lpath(new QLabel("Path : "))
    ,iconLabel(new QLabel)
    ,pathLabel(new QLabel)
    ,bookMarkName(new QLineEdit)
    ,sectionName(new QComboBox)
{
    qDebug() << "bookmarkDialog opening";
    setWindowTitle("Add to Bookmark");
    setWindowIcon(QIcon(":/icons/Bookmarks.svg"));

    statusLabel->setStyleSheet("color:rgb(255,0,0)");
    QVBoxLayout *vertical = new QVBoxLayout();
    QHBoxLayout *horizontal_0 = new QHBoxLayout();
    horizontal_0->setContentsMargins(2, 2, 2, 2);
    QHBoxLayout *horizontal_1 = new QHBoxLayout();
    QHBoxLayout *horizontal_2 = new QHBoxLayout();
    QHBoxLayout *horizontal_22 = new QHBoxLayout();
    QHBoxLayout *horizontal_3 = new QHBoxLayout();
    QSpacerItem *hSpacer = new QSpacerItem(40, 40, QSizePolicy::Expanding, QSizePolicy::Expanding);

    lpath->setWordWrap(true);

    horizontal_1->addWidget(lSection);
    horizontal_1->addWidget(sectionName);

    horizontal_2->addWidget(lbkName);
    horizontal_2->addWidget(bookMarkName);

    horizontal_22->addWidget(lpath);
    horizontal_22->addWidget(pathLabel);

    accept->setEnabled(false);
    horizontal_3->addSpacerItem(hSpacer);
    horizontal_3->addWidget(cancel);
    horizontal_3->addWidget(accept);

    vertical->addLayout(horizontal_1);
    vertical->addLayout(horizontal_2);
    vertical->addLayout(horizontal_22);
    vertical->addWidget(statusLabel);
    vertical->addLayout(horizontal_3);

    horizontal_0->addWidget(iconLabel);
    iconLabel->setMaximumSize(120, 120);
    horizontal_0->addLayout(vertical);

    setLayout(horizontal_0);

    QShortcut *shortcut = new QShortcut(QKeySequence(Qt::Key_Enter), this);

    connect(accept, SIGNAL(pressed()), this, SLOT(ok_clicked()));
    connect(cancel, SIGNAL(pressed()), this, SLOT(close()));

    connect(shortcut, &QShortcut::activated, this, &bookmarkDialog::ok_clicked);
    accept->setShortcut(QKeySequence(Qt::Key_Enter));

    connect(bookMarkName, SIGNAL(textChanged(QString)), this, SLOT(bookMarkName_Changed()));
    connect(sectionName, SIGNAL(currentIndexChanged(QString)), this, SLOT(item_Changed()));

    sectionName->clear();
    sectionName->addItems(bk.getBookSections());
    accept->setEnabled(false);
}

bookmarkDialog::~bookmarkDialog()
{
    qDebug()<<"bookmarkdialog closing";
    delete accept;
    delete cancel;
    delete lSection;
    delete lbkName;
    delete bookMarkName;
    delete sectionName;
    delete iconLabel;
    delete statusLabel;
    delete pathLabel;
}

void bookmarkDialog::ok_clicked()
{
    if (bookMarkName->text().count() == 0) {
        accept->setEnabled(false);
    }
    if (!bookMarkName->text().count() == 0 && !sectionName->currentText().count() == 0) {
        accepted = true;
        QTimer::singleShot(100, this, SLOT(close()));
        messageEngine("Bookmark Added at '" + sectionName->currentText() + "'", "Info");
    }
}

void bookmarkDialog::bookMarkName_Changed()
{
    if (bookMarkName->text().count() > 0) {
        QString str = bk.checkingBookName(sectionName->currentText(), bookMarkName->text());
        if (str.count() > 0) {
            statusLabel->setText(str);
            accept->setEnabled(false);
        } else {
            statusLabel->setText(str);
            accept->setEnabled(true);
        }
    }
    else {
        accept->setEnabled(false);
    }
}

void bookmarkDialog::checkPath()
{
    QString str = bk.checkingBookPath(sectionName->currentText(), pathLabel->text());
    if (str.count() > 0) {
        statusLabel->setText(str);
        bookMarkName->setEnabled(false);
        accept->setEnabled(false);
        cancel->setText("OK");
    } else {
        statusLabel->setText(str);
        bookMarkName->setEnabled(true);
        accept->setEnabled(true);
        cancel->setText("Cancel");
    }
}

void bookmarkDialog::item_Changed()
{
    checkPath();
    bookMarkName_Changed();
}
