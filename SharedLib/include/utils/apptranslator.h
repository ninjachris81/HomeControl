#ifndef APPTRANSLATOR_H
#define APPTRANSLATOR_H

#include <QObject>
#include <QTranslator>
#include <QGuiApplication>

#include "utils/appconfiguration.h"

class AppTranslator : public QObject
{
    Q_OBJECT
public:
    explicit AppTranslator(AppConfiguration *appConfig, QObject *parent = nullptr);

    void switchTranslator(QTranslator& translator, const QString& filename);
    void setLanguage(const QString& lang);

private:
    AppConfiguration *m_appConfig;
    QString m_currentLang;

    QTranslator m_translator;

signals:

public slots:
};

#endif // APPTRANSLATOR_H
