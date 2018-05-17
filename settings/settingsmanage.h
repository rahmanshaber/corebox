#ifndef SETTINGSMANAGE_H
#define SETTINGSMANAGE_H

#include <QSettings>
#include <QVariant>


class SettingsManage
{
public:
    SettingsManage();
    QSettings *cSetting;
    void createDefaultSettings();

    bool setSpecificValue(QString groupName, QString keyName, QVariant value);
    QVariant getSpecificValue(QString groupName, QString keyName);

    QString getTerminal();
    bool setTerminal(QString termName);

    QString getStartupPath();
    bool setStartupPath(QString path);

    QString getThemeName();
    bool setThemeName(QString themeName);

    bool getIsRealMimeType();
    bool setIsRealMimeType(bool isRealMime);

    int getZoomValue();
    bool setZoomValue(int value);

    int getZoomTreeValue();
    bool setZoomTreeValue(int value);

    int getZoomListValue();
    bool setZoomListValue(int value);

    int getZoomDetailValue();
    bool setZoomDetailValue(int value);

    bool getIsShowThumb();
    bool setIsShowThumb(bool isShowThumb);

    bool getShowToolbox();
    bool setShowToolbox(bool showTool);

    bool getViewMode();
    bool setViewMode(bool mode);

    QString getSCSaveLocation();
    bool setSCSaveLocation(QString path);

    bool getBoxIsMaximize();
    bool setBoxIsMaximize(bool isMaximize);

    bool getShowBattery();
    bool setSHowBattery(bool showBattery);

    bool getDisableRecent();
    bool setDisableRecent(bool showRecent);

    QString getTimeFormat();
    bool setTimeFormat(QString AmPm);

    bool getShowHidden();
    bool setShowHidden(bool showTool);

    bool getShowCalander();
    bool setSHowCalander(bool showBattery);

    bool getShowSystem();
    bool setSHowSystem(bool showSystem);

    bool getShowNetwork();
    bool setSHowNetwork(bool showNetwork);

    bool getShowCalculator();
    bool setSHowCalculator(bool showCalculator);

    bool getShowNote();
    bool setSHowNote(bool showNote);

    bool setSHowTime(bool showTime);
    bool getShowTime();
};

#endif // SETTINGSMANAGE_H
