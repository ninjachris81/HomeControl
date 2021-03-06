#ifndef SETTINGSCONTROLLER_H
#define SETTINGSCONTROLLER_H

#include "controllerbase.h"
#include <QSettings>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(LG_SETTINGS_CONTROLLER)

class SettingsController : public ControllerBase
{
public:
    SettingsController(QObject *parent = nullptr);

    QString getName();

    CONTROLLER_TYPE getType() {
        return SETTINGS_CONTROLLER;
    }

    QStringList getTopicPath();

    QStringList getLabelList();

    QList<QVariant::Type> getValueTypes();

    QString getEnumName();

    QVariant::Type getDefaultValueType();

    qint64 getValueLifetime(int index = -1);

    static QString CONTROLLER_NAME;

protected:
    bool hasSetSupport() {
        return true;
    }

    void publishSettingsValue(EnumsDeclarations::MQTT_SETTINGS key, QVariant defaultValue);

    void onInit();

    void onValueChanged(int index, QVariant value);

    void onConnectedChanged(bool connected);

    QSettings *m_settings;

    QVariant getSettingsValue(int index, QVariant defaultValue, bool createIfNotExists = true);

    QString getSettingsKey(int index);

};

#endif // SETTINGSCONTROLLER_H
