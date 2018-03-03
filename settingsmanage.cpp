#include "settingsmanage.h"

#include <QDir>
#include <QIcon>
#include <QDebug>
#include <QFileInfo>

SettingsManage::SettingsManage()
{
    qDebug() << "SettingsManage opening";
    cSetting = new QSettings("coreBox", "coreBox");
}

void SettingsManage::createDefaultSettings() {
    qDebug() << "createDefaultSettings opening";
    if (!QFileInfo(cSetting->fileName()).exists()) {
        cSetting->beginGroup("CoreFM");
        cSetting->setValue("Terminal", "xfce4-terminal");
        cSetting->setValue("Startup-Path", QDir::homePath());
        cSetting->setValue("Force-Theme", QIcon::themeName());
        cSetting->setValue("Default-Mime-Apps-File", "/.local/share/applications/mimeapps.ui->viewlist");
        cSetting->setValue("Real-Mime-Types", true);
        cSetting->setValue("Zoom", 48);
        cSetting->setValue("Zoom-Tree", 16);
        cSetting->setValue("Zoom-List", 24);
        cSetting->setValue("Zoom-Detail", 16);
        cSetting->setValue("Show-Thumb", true);
        cSetting->setValue("Show-Toolbox", false);
        cSetting->setValue("View-Mode", "Detail");
        cSetting->setValue("Sort-Column", 0);
        cSetting->setValue("Sort-Order", 0);
        cSetting->endGroup();

        cSetting->beginGroup("CoreScreenshot");
        cSetting->setValue("Save-Location", QDir::homePath() + "/Pictures");
        cSetting->endGroup();

        cSetting->beginGroup("CoreBox");
        cSetting->setValue("Maximized", false);
        cSetting->setValue("Show-Battery", true);
        cSetting->setValue("Recent-Disable", false);
        cSetting->endGroup();
    }
}

bool SettingsManage::setSpecificValue(QString groupName, QString keyName, QVariant value) {
    if (!groupName.isEmpty() && !keyName.isEmpty()) {
        cSetting->beginGroup(groupName);
        cSetting->setValue(keyName, value);
        cSetting->endGroup();
        return true;
    }
    return false;
}

QVariant SettingsManage::getSpecificValue(QString groupName, QString keyName) {
    if (!groupName.isEmpty() && !keyName.isEmpty()) {
        cSetting->beginGroup(groupName);
        QVariant v = cSetting->value(keyName);
        cSetting->endGroup();
        return v;
    }
    return QVariant();
}

bool SettingsManage::setTerminal(QString termName) {
    return setSpecificValue("CoreFM", "Terminal", termName);
}

QString SettingsManage::getTerminal() {
    return getSpecificValue("CoreFM", "Terminal").toString();
}

bool SettingsManage::setThemeName(QString themeName) {
    return setSpecificValue("CoreFM", "Force-Theme", themeName);
}

QString SettingsManage::getThemeName() {
    return getSpecificValue("CoreFM", "Force-Theme").toString();
}

bool SettingsManage::setStartupPath(QString path) {
    return setSpecificValue("CoreFM", "Startup-Path", path);
}

QString SettingsManage::getStartupPath() {
    return getSpecificValue("CoreFM", "Startup-Path").toString();
}

bool SettingsManage::setMimeFilePath(QString mimeFilePath) {
    return setSpecificValue("CoreFM", "Default-Mime-Apps-File", mimeFilePath);
}

QString SettingsManage::getMimeFilePath() {
    return getSpecificValue("CoreFM", "Default-Mime-Apps-File").toString();
}

bool SettingsManage::setIsRealMimeType(bool isRealMime) {
    return setSpecificValue("CoreFM", "Real-Mime-Types", isRealMime);
}

bool SettingsManage::getIsRealMimeType() {
    return getSpecificValue("CoreFM", "Real-Mime-Types").toBool();
}

bool SettingsManage::setZoomValue(int value) {
    return setSpecificValue("CoreFM", "Zoom", value);
}

int SettingsManage::getZoomValue() {
    return getSpecificValue("CoreFM", "Zoom").toInt();
}

bool SettingsManage::setZoomTreeValue(int value) {
    return setSpecificValue("CoreFM", "Zoom-Tree", value);
}

int SettingsManage::getZoomTreeValue() {
    return getSpecificValue("CoreFM", "Zoom-Tree").toInt();
}

bool SettingsManage::setZoomListValue(int value) {
    return setSpecificValue("CoreFM", "Zoom-List", value);
}

int SettingsManage::getZoomListValue() {
    return getSpecificValue("CoreFM", "Zoom-List").toInt();
}

bool SettingsManage::setZoomDetailValue(int value) {
    return setSpecificValue("CoreFM", "Zoom-Detail", value);
}

int SettingsManage::getZoomDetailValue() {
    return getSpecificValue("CoreFM", "Zoom-Detail").toInt();
}

bool SettingsManage::setIsShowThumb(bool isShowThumb) {
    return setSpecificValue("CoreFM", "Show-Thumb", isShowThumb);
}

bool SettingsManage::getIsShowThumb() {
    return getSpecificValue("CoreFM", "Show-Thumb").toBool();
}

bool SettingsManage::setShowToolbox(bool showTool) {
    return setSpecificValue("CoreFM", "Show-Toolbox", showTool);
}

bool SettingsManage::getShowToolbox() {
    return getSpecificValue("CoreFM", "Show-Toolbox").toBool();
}

bool SettingsManage::setViewMode(QString mode) {
    return setSpecificValue("CoreFM", "View-Mode", mode);
}

QString SettingsManage::getViewMode() {
    return getSpecificValue("CoreFM", "View-Mode").toString();
}

bool SettingsManage::setSortColumn(int value) {
    return setSpecificValue("CoreFM", "Sort-Column", value);
}

int SettingsManage::getSortColumn() {
    return getSpecificValue("CoreFM", "Sort-Column").toInt();
}

bool SettingsManage::setSortOrder(Qt::SortOrder order) {
    return setSpecificValue("CoreFM", "Sort-Order", order);
}

int SettingsManage::getSortOrder() {
    return getSpecificValue("CoreFM", "Sort-Order").toInt();
}

bool SettingsManage::setSCSaveLocation(QString path) {
    return setSpecificValue("CoreScreenshot", "Save-Location", path);
}

QString SettingsManage::getSCSaveLocation() {
    return getSpecificValue("CoreScreenshot", "Save-Location").toString();
}

bool SettingsManage::setBoxIsMaximize(bool isMaximize) {
    return setSpecificValue("CoreBox", "Maximized", isMaximize);
}

bool SettingsManage::getBoxIsMaximize() {
    return getSpecificValue("CoreBox", "Maximized").toBool();
}

bool SettingsManage::setSHowBattery(bool showBattery) {
    return setSpecificValue("CoreBox", "Show-Battery", showBattery);
}

bool SettingsManage::getShowBattery() {
    return getSpecificValue("CoreBox", "Show-Battery").toBool();
}

bool SettingsManage::setDisableRecent(bool showRecent) {
    return setSpecificValue("CoreBox", "Recent-Disable", showRecent);
}

bool SettingsManage::getDisableRecent() {
    return getSpecificValue("CoreBox", "Recent-Disable").toBool();
}
