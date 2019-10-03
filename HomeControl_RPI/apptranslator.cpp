#include "apptranslator.h"

#include <QDebug>
#include <QLibraryInfo>

AppTranslator::AppTranslator(AppConfiguration *appConfig, QObject *parent) : QObject(parent), m_appConfig(appConfig)
{
    QString lang = m_appConfig->getString("UI_LANGUAGE", "en");
    setLanguage(lang);
}

void AppTranslator::switchTranslator(QTranslator& translator, const QString& filename)
{
    qDebug() << Q_FUNC_INFO << filename;

    // remove the old translator
    qApp->removeTranslator(&translator);

    // load the new translator
    if(translator.load(filename)) {
        qApp->installTranslator(&translator);
    } else {
        qWarning() << "Failed to load" << filename;
    }
}

void AppTranslator::setLanguage(const QString& lang) {
    qDebug() << Q_FUNC_INFO << lang;

    if(m_currentLang == lang) return;

    m_currentLang = lang;
    QLocale locale = QLocale(m_currentLang);
    QLocale::setDefault(locale);
    switchTranslator(m_translator, QString(":/languages/hc.%1.qm").arg(lang));
}
