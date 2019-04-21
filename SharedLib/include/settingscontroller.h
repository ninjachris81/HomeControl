#ifndef SETTINGSCONTROLLER_H
#define SETTINGSCONTROLLER_H

#include "controllerbase.h"
#include <QSettings>

class SettingsController : public ControllerBase
{
public:
    enum SETTINGS_MODE {
        SETTINGS_SERVER,
        SETTINGS_CLIENT
    };

    SettingsController(SETTINGS_MODE mode, QObject *parent = nullptr);

    QString getName();

    QStringList getTopicPath();

    QStringList getLabelList();

    QVariant::Type getValueType(int index);

    static QString CONTROLLER_NAME;

protected:
    SETTINGS_MODE m_mode;

    bool hasSetSupport() {
        return true;
    }

    void onInit();

    void onValueChanged(int index, QVariant value);

    void onConnectedChanged(bool connected);

    QSettings *m_settings;

    QVariant getSettingsValue(int index, QVariant defaultValue, bool createIfNotExists = true);

    QString getSettingsKey(int index);

};

#endif // SETTINGSCONTROLLER_H
