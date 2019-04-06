#ifndef APPCONFIGURATION_H
#define APPCONFIGURATION_H

#include <QObject>
#include <QSettings>

class AppConfiguration : public QObject
{
    Q_OBJECT
public:
    explicit AppConfiguration(QObject *parent = nullptr);
    ~AppConfiguration();

    enum ConfigurationKey {
        APP_VERSION,
        MQTT_HOST,
        MQTT_PORT
    };
    Q_ENUM(ConfigurationKey)

    QVariant getValue(ConfigurationKey key, QVariant defaultValue);

    QString getString(ConfigurationKey key, QString defaultValue);

    int getInt(ConfigurationKey key, int defaultValue);

private:
    QSettings* m_settings;

    QString keyToString(ConfigurationKey key);

signals:

public slots:
};

#endif // APPCONFIGURATION_H
