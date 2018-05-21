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

#include "settings.h"
#include "ui_settings.h"

#include <QDir>
#include <QFile>
#include <QIcon>
#include <QFileDialog>
#include <QDebug>
#include <QTreeWidgetItem>


settings::settings(QWidget *parent) :QWidget(parent),ui(new Ui::settings)
{
    qDebug() << "settings opening";
    ui->setupUi(this);

    setupCoreBoxPage();
    setupCoreFMPage();
    setupCoreActionPage();
    setupCoreShotPage();

}

settings::~settings()
{
    qDebug()<<"settings closing";
    delete ui;
}

void settings::setupCoreBoxPage()
{
    //general
    ui->isMaximized->setChecked(sm.getBoxIsMaximize());
    ui->isRecentDisable->setChecked(sm.getDisableRecent());

    //looks & feel
    QString currentTheme = sm.getThemeName();
    QDirIterator it("/usr/share/icons", QDir::Dirs | QDir::NoDotAndDotDot);
    QStringList iconThemes;
    while (it.hasNext()) {
      it.next();
      iconThemes.append(it.fileName());
    }
    ui->cmbIconTheme->addItems(iconThemes);
    ui->cmbIconTheme->setCurrentText(currentTheme);
}

void settings::setupCoreActionPage()
{
    //show widgets
    ui->isTime->setChecked(sm.getShowTime());
    ui->isBattery->setChecked(sm.getShowBattery());
    ui->isSystem->setChecked(sm.getShowSystem());
    ui->isNetwork->setChecked(sm.getShowNetwork());
    ui->isCalander->setChecked(sm.getShowCalander());
    ui->isCalculator->setChecked(sm.getShowCalculator());
    ui->isNotes->setChecked(sm.getShowNote());
}

void settings::setupCoreFMPage()
{
    //general
    ui->checkThumbs->setChecked(sm.getIsShowThumb());
    ui->showTool->setChecked(sm.getShowToolbox());
    ui->startPath->setText(sm.getStartupPath());
    ui->showrealmime->setChecked(sm.getIsRealMimeType());

    ui->view->addItem("Detail");
    ui->view->addItem("Icon");
    ui->view->setCurrentIndex(sm.getViewMode());

    //terminal
    selectedTerminal = ui->terminals->currentText();
    ui->terminals->setCurrentText(sm.getTerminal());

    //setup mimes
    MimeUtils *mimeUtils = new MimeUtils(this);
    QString tmp = "/.config/coreBox/mimeapps.list";
    mimeUtils->setDefaultsFileName(tmp);
    QStringList mimes = mimeUtils->getMimeTypes();

    QIcon defaultIcon = QIcon::fromTheme("unknown");

    QMap<QString, QTreeWidgetItem*> categories;
    QMap<QTreeWidgetItem*, QIcon> genericIcons;

    foreach (QString mime, mimes) {

      // Updates progress
      //progressMime->setValue(progressMime->value() + 1);

      // Skip all 'inode' nodes including 'inode/directory'
      if (mime.startsWith("inode")) {
        continue;
      }

      // Skip all 'x-content' and 'message' nodes
      if (mime.startsWith("x-content") || mime.startsWith("message")) {
        continue;
      }

      // Parse mime
      QStringList splitMime = mime.split("/");

      // Retrieve cathegory
      QIcon icon;
      QString categoryName = splitMime.first();
      QTreeWidgetItem* category = categories.value(categoryName, NULL);
      if (!category) {
        category = new QTreeWidgetItem(ui->mimesWidget);
        category->setText(0, categoryName);
        category->setFlags(Qt::ItemIsEnabled);
        categories.insert(categoryName, category);
        icon = FileUtils::searchGenericIcon(categoryName, defaultIcon);
        genericIcons.insert(category, icon);
      } else {
        icon = genericIcons.value(category);
      }

      // Load icon and default application for current mime
      // NOTE: if icon is not found generic icon is used
      icon = FileUtils::searchMimeIcon(mime, icon);
      QString appNames = mimeUtils->getDefault(mime).join(";");

      // Create item from current mime
      QTreeWidgetItem *item = new QTreeWidgetItem(category);
      item->setIcon(0, icon);
      item->setText(0, splitMime.at(1));
      item->setText(1, appNames.remove(".desktop"));
      item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    }

    // Load application list
    QStringList apps = FileUtils::getApplicationNames();
    apps.replaceInStrings(".desktop", "");
    apps.sort();

    // Prepare source of icons
    QDir appIcons("/usr/share/pixmaps","", 0, QDir::Files | QDir::NoDotAndDotDot);
    QStringList iconFiles = appIcons.entryList();
    QIcon defaulticon = QIcon::fromTheme("application-x-executable");

    // Loads icon list
    QList<QIcon> icons;
    foreach (QString app, apps) {
      QPixmap temp = QIcon::fromTheme(app).pixmap(16, 16);
      if (!temp.isNull()) {
        icons.append(temp);
      } else {
        QStringList searchIcons = iconFiles.filter(app);
        if (searchIcons.count() > 0) {
          icons.append(QIcon("/usr/share/pixmaps/" + searchIcons.at(0)));
        } else {
          icons.append(defaulticon);
        }
      }
    }

    // Connect
    connect(ui->mimesWidget,
            SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
            SLOT(onMimeSelected(QTreeWidgetItem*,QTreeWidgetItem*)));
    connect(ui->btnAdd, SIGNAL(clicked()), SLOT(showAppDialog()));
    connect(ui->removeAppAssoc, SIGNAL(clicked()), SLOT(removeAppAssoc()));
    connect(ui->moveAppAssocUp, SIGNAL(clicked()), SLOT(moveAppAssocUp()));
    connect(ui->moveAppAssocDown, SIGNAL(clicked()), SLOT(moveAppAssocDown()));

}

void settings::setupCoreShotPage()
{
    //general
    ui->ssLocation->setText(sm.getSCSaveLocation());
}

void settings::onMimeSelected(QTreeWidgetItem *current,QTreeWidgetItem *previous)
{
    Q_UNUSED(current);
    // Store previously used associations
    updateMimeAssoc(previous);

    // Clear previously used associations
    ui->listAssoc->clear();

    // Check if current is editable
//    if (current->childCount() > 0) {
//      grpAssoc->setEnabled(false);
//      return;
//    }

    // Enable editation
    //grpAssoc->setEnabled(true);

    // Prepare source of icons
    QDir appIcons("/usr/share/pixmaps","", 0, QDir::Files | QDir::NoDotAndDotDot);
    QStringList iconFiles = appIcons.entryList();
    QIcon defaultIcon = QIcon::fromTheme("application-x-executable");

    QStringList apps = ui->mimesWidget->currentItem()->text(1).remove(" ").split(";");
    foreach (QString app, apps) {

      // Skip empty string
      if (app.compare("") == 0) {
        continue;
      }

      // Finds icon
      QIcon temp = QIcon::fromTheme(app).pixmap(16, 16);
      if (temp.isNull()) {
        QStringList searchIcons = iconFiles.filter(app);
        if (searchIcons.count() > 0) {
          temp = QIcon("/usr/share/pixmaps/" + searchIcons.at(0));
        } else {
          temp = defaultIcon;
        }
      }

      // Add application
      ui->listAssoc->addItem(new QListWidgetItem(temp, app, ui->listAssoc));
    }

}

void settings::updateMimeAssoc(QTreeWidgetItem* item)
{
    if (item && item->childCount() == 0) {
      QStringList associations;
      for (int i = 0; i < ui->listAssoc->count(); i++) {
        associations.append(ui->listAssoc->item(i)->text());
      }
      item->setText(1, associations.join(";"));
    }
}

void settings::showAppDialog()
{
    // Choose application
    ApplicationDialog *dialog = new ApplicationDialog(this);
    if (dialog->exec()) {

      // If application name is empty, exit
      if (dialog->getCurrentLauncher().isEmpty()) {
        return;
      }

      // Retrieve launcher name
      QString name = dialog->getCurrentLauncher();

      // If application with same name is already used, exit
      for (int i = 0; i < ui->listAssoc->count(); i++) {
        if (ui->listAssoc->item(i)->text().compare(name) == 0) {
          return;
        }
      }

      // Add new launcher to the list of launchers
      if (dialog->getCurrentLauncher().compare("") != 0) {
        QIcon icon = QIcon::fromTheme(name).pixmap(16, 16);
        ui->listAssoc->addItem(new QListWidgetItem(icon, name, ui->listAssoc));
        updateMimeAssoc(ui->mimesWidget->currentItem());
      }
    }
}

/**
 * @brief Removes association of application and mime type
 */
void settings::removeAppAssoc()
{
    qDeleteAll(ui->listAssoc->selectedItems());
    updateMimeAssoc(ui->mimesWidget->currentItem());
}

/**
 * @brief Moves association up in list
 */
void settings::moveAppAssocUp()
{
    QListWidgetItem *current = ui->listAssoc->currentItem();
    int currIndex = ui->listAssoc->row(current);
    QListWidgetItem *prev = ui->listAssoc->item(ui->listAssoc->row(current) - 1);
    int prevIndex = ui->listAssoc->row(prev);
    QListWidgetItem *temp = ui->listAssoc->takeItem(prevIndex);
    ui->listAssoc->insertItem(prevIndex, current);
    ui->listAssoc->insertItem(currIndex, temp);
    updateMimeAssoc(ui->mimesWidget->currentItem());
}

/**
 * @brief Moves association down in list
 */
void settings::moveAppAssocDown()
{
    QListWidgetItem *current = ui->listAssoc->currentItem();
    int currIndex = ui->listAssoc->row(current);
    QListWidgetItem *next = ui->listAssoc->item(ui->listAssoc->row(current) + 1);
    int nextIndex = ui->listAssoc->row(next);
    QListWidgetItem *temp = ui->listAssoc->takeItem(nextIndex);
    ui->listAssoc->insertItem(currIndex, temp);
    ui->listAssoc->insertItem(nextIndex, current);
    updateMimeAssoc(ui->mimesWidget->currentItem());
}

void settings::on_terminals_currentIndexChanged(const QString &arg1)
{
    selectedTerminal = arg1;
}

void settings::on_cancel_clicked()
{
    CoreBox *cBox = qobject_cast<CoreBox*>(qApp->activeWindow());
    cBox->closeCurrentTab();
    close();
    deleteLater();
}

void settings::on_ok_clicked()
{
    //corebox
    sm.setBoxIsMaximize(ui->isMaximized->isChecked());
    sm.setDisableRecent(ui->isRecentDisable->isChecked());
    sm.setThemeName(ui->cmbIconTheme->currentText());

    //corefm
    MimeUtils *mimeUtils = new MimeUtils(this);
    QString tmp = "/.config/coreBox/mimeapps.list";
    mimeUtils->setDefaultsFileName(tmp);
    for (int i = 0; i < ui->mimesWidget->topLevelItemCount(); ++i) {
      QTreeWidgetItem* cathegory = ui->mimesWidget->topLevelItem(i);
      QString cathegoryName = cathegory->text(0) + "/";
      for (int j = 0; j < cathegory->childCount(); j++) {
        QString mime = cathegoryName + cathegory->child(j)->text(0);
        QString appNames = cathegory->child(j)->text(1);
        if (!appNames.isEmpty()) {
          QStringList temps = appNames.split(";");
          for (int i = 0; i < temps.size(); i++) {
            temps[i] = temps[i] + ".desktop";
          }
          mimeUtils->setDefault(mime, temps);
        }
      }
    }
    mimeUtils->saveDefaults();

    sm.setTerminal(ui->terminals->currentText());
    sm.setStartupPath(ui->startPath->text());
    sm.setIsRealMimeType(ui->showrealmime->isChecked());
    sm.setIsShowThumb(ui->checkThumbs->isChecked());
    sm.setShowToolbox(ui->showTool->isChecked());
    sm.setViewMode(ui->view->currentIndex() == 0 ? "Detail" : "Icon");

    //corescreenshot
    sm.setSCSaveLocation(ui->ssLocation->text());

    //coreaction
    sm.setSHowTime(ui->isTime->isChecked());
    sm.setSHowBattery(ui->isBattery->isChecked());
    sm.setSHowSystem(ui->isSystem->isChecked());
    sm.setSHowNetwork(ui->isNetwork->isChecked());
    sm.setSHowCalander(ui->isCalander->isChecked());
    sm.setSHowCalculator(ui->isCalculator->isChecked());
    sm.setSHowNote(ui->isNotes->isChecked());

    //inform the user
    messageEngine("Settings Applied\nCoreBox needs to restart", "Info");
    QIcon::setThemeName(sm.getThemeName());
}

void settings::on_browseF_clicked()
{
    ui->startPath->setText(QFileDialog::getExistingDirectory(this, "Select Startup Path for CoreFM"));
}

void settings::on_browseS_clicked()
{
    ui->ssLocation->setText(QFileDialog::getExistingDirectory(this, "Select Save Location for Screenshot"));
}

void settings::pageClick(QPushButton *btn, int i, QString title)
{
    // all button checked false
    for (QPushButton *b : ui->shortcut->findChildren<QPushButton*>())
        b->setChecked(false);
    btn->setChecked(true);
    ui->selectedsection->setText(title);
    ui->pages->setCurrentIndex(i);
}

void settings::on_corebox_clicked()
{
    pageClick(ui->corebox,0, tr("CoreBox"));
}

void settings::on_coreaction_clicked()
{
    pageClick(ui->coreaction,1, tr("CoreAction"));
}

void settings::on_corefm_clicked()
{
    pageClick(ui->corefm,2, tr("CoreFM"));
}

void settings::on_corescrshot_clicked()
{
    pageClick(ui->corescrshot,3, tr("CoreScreenShot"));
}
