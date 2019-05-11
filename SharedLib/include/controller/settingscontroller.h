#ifndef SETTINGSCONTROLLER_H
#define SETTINGSCONTROLLER_H

#include "controllerbase.h"
#include <QSettings>

class SettingsController : public ControllerBase
{
public:
    SettingsController(QObject *parent = nullptr);

    QString getName();

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

    void onInit();

    void onValueChanged(int index, QVariant value);

    void onConnectedChanged(bool connected);

    QSettings *m_settings;

    QVariant getSettingsValue(int index, QVariant defaultValue, bool createIfNotExists = true);

    QString getSettingsKey(int index);

};

#endif // SETTINGSCONTROLLER_H
