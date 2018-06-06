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

#include "settingsmanage.h"

#include <QDir>
#include <QIcon>
#include <QFileInfo>


SettingsManage::SettingsManage()
{
    cSetting = new QSettings("coreBox", "coreBox");
}

void SettingsManage::createDefaultSettings()
{
    if (!QFileInfo(cSetting->fileName()).exists()) {

        cSetting->beginGroup("CoreBox");
        cSetting->setValue("Maximized", false);
        cSetting->setValue("Recent-Disable", true);
        cSetting->setValue("Force-Theme", QIcon::themeName());
        cSetting->endGroup();

        cSetting->beginGroup("CoreFM");
        cSetting->setValue("Terminal", "xterm");
        cSetting->setValue("Startup-Path", QDir::homePath());
        cSetting->setValue("Real-Mime-Types", true);
        cSetting->setValue("Zoom", 80);
        cSetting->setValue("Zoom-Tree", 16);
        cSetting->setValue("Zoom-List", 24);
        cSetting->setValue("Zoom-Detail", 24);
        cSetting->setValue("Show-Thumb", false);
        cSetting->setValue("Show-Toolbox", false);
        cSetting->setValue("Show-Hidden",false);
        cSetting->setValue("View-Mode", true);
        cSetting->endGroup();

        cSetting->beginGroup("CoreScreenshot");
        cSetting->setValue("Save-Location", QDir::homePath() + "/Pictures");
        cSetting->endGroup();

        cSetting->beginGroup("CoreAction");
        cSetting->setValue("Show-Time", true);
        cSetting->setValue("Show-Battery", false);
        cSetting->setValue("Show-System", false);
        cSetting->setValue("Show-Network", false);
        cSetting->setValue("Show-Calander", false);
        cSetting->setValue("Show-Calculator", true);
        cSetting->setValue("Show-Note", true);
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

//-------------------------CoreBox--------------------------------------

bool SettingsManage::setBoxIsMaximize(bool isMaximize) {
    return setSpecificValue("CoreBox", "Maximized", isMaximize);
}

bool SettingsManage::getBoxIsMaximize() {
    return getSpecificValue("CoreBox", "Maximized").toBool();
}

bool SettingsManage::setDisableRecent(bool showRecent) {
    return setSpecificValue("CoreBox", "Recent-Disable", showRecent);
}

bool SettingsManage::getDisableRecent() {
    return getSpecificValue("CoreBox", "Recent-Disable").toBool();
}

//-------------------------CoreBox--------------------------------------

//-------------------------CoreFM--------------------------------------

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

bool SettingsManage::setShowHidden(bool showTool) {
    return setSpecificValue("CoreFM", "Show-Hidden", showTool);
}

bool SettingsManage::getShowHidden() {
    return getSpecificValue("CoreFM", "Show-Hidden").toBool();
}

bool SettingsManage::setViewMode(bool mode) {
    return setSpecificValue("CoreFM", "View-Mode", mode);
}

bool SettingsManage::getViewMode() {
    return getSpecificValue("CoreFM", "View-Mode").toBool();
}

//-------------------------CoreFM--------------------------------------

//-------------------------CoreScreenshot------------------------------

bool SettingsManage::setSCSaveLocation(QString path) {
    return setSpecificValue("CoreScreenshot", "Save-Location", path);
}

QString SettingsManage::getSCSaveLocation() {
    return getSpecificValue("CoreScreenshot", "Save-Location").toString();
}

//-------------------------CoreScreenshot------------------------------

//-------------------------CoreAction------------------------------

bool SettingsManage::setSHowTime(bool showTime) {
    return setSpecificValue("CoreAction", "Show-Time", showTime);
}

bool SettingsManage::getShowTime() {
    return getSpecificValue("CoreAction", "Show-Time").toBool();
}

bool SettingsManage::setSHowBattery(bool showBattery) {
    return setSpecificValue("CoreAction", "Show-Battery", showBattery);
}

bool SettingsManage::getShowBattery() {
    return getSpecificValue("CoreAction", "Show-Battery").toBool();
}

bool SettingsManage::setSHowSystem(bool showSystem) {
    return setSpecificValue("CoreAction", "Show-System", showSystem);
}

bool SettingsManage::getShowSystem() {
    return getSpecificValue("CoreAction", "Show-System").toBool();
}

bool SettingsManage::setSHowNetwork(bool showNetwork) {
    return setSpecificValue("CoreAction", "Show-Network", showNetwork);
}

bool SettingsManage::getShowNetwork() {
    return getSpecificValue("CoreAction", "Show-Network").toBool();
}

bool SettingsManage::setSHowCalander(bool showCalander) {
    return setSpecificValue("CoreAction", "Show-Calander", showCalander);
}

bool SettingsManage::getShowCalander() {
    return getSpecificValue("CoreAction", "Show-Calander").toBool();
}

bool SettingsManage::setSHowCalculator(bool showCalculator) {
    return setSpecificValue("CoreAction", "Show-Calculator", showCalculator);
}

bool SettingsManage::getShowCalculator() {
    return getSpecificValue("CoreAction", "Show-Calculator").toBool();
}

bool SettingsManage::setSHowNote(bool showNote) {
    return setSpecificValue("CoreAction", "Show-Note", showNote);
}

bool SettingsManage::getShowNote() {
    return getSpecificValue("CoreAction", "Show-Note").toBool();
}

//-------------------------CoreAction------------------------------
