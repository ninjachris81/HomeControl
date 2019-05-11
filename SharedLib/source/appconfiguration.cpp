#include "include/appconfiguration.h"
#include <QDebug>
#include <QFile>

AppConfiguration::AppConfiguration(QObject *parent) : QObject(parent)
{
    m_settings = new QSettings("config.ini", QSettings::IniFormat);
    qDebug() << "Config file" << m_settings->fileName();
    if (!QFile(m_settings->fileName()).exists()) {
        qDebug() << "Creating new config file";
        m_settings->setValue(keyToString(APP_VERSION), 1);
        m_settings->sync();
    }
}

AppConfiguration::~AppConfiguration() {
    m_settings->sync();
}

QString AppConfiguration::keyToString(ConfigurationKey key) {
    return QVariant::fromValue(key).toString();
}

QVariant AppConfiguration::getValue(ConfigurationKey key, QVariant defaultValue) {
    return getValue(keyToString(key), defaultValue);
}

QString AppConfiguration::getString(ConfigurationKey key, QString defaultValue) {
    return getString(keyToString(key), defaultValue);
}

int AppConfiguration::getInt(ConfigurationKey key, int defaultValue) {
    return getInt(keyToString(key), defaultValue);
}

QVariant AppConfiguration::getValue(QString key, QVariant defaultValue) {
    if (m_settings->contains(key)) {
        return m_settings->value(key);
    } else {
        m_settings->setValue(key, defaultValue);
        return defaultValue;
    }
}

QString AppConfiguration::getString(QString key, QString defaultValue) {
    if (m_settings->contains(key)) {
        return m_settings->value(key).toString();
    } else {
        m_settings->setValue(key, defaultValue);
        return defaultValue;
    }
}

int AppConfiguration::getInt(QString key, int defaultValue) {
    if (m_settings->contains(key)) {
        return m_settings->value(key).toInt();
    } else {
        m_settings->setValue(key, defaultValue);
        return defaultValue;
    }
}
