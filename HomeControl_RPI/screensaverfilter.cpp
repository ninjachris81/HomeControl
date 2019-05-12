#include "screensaverfilter.h"
#include <QTouchEvent>
#include <QMouseEvent>
#include <QDebug>
#include <QProcess>

ScreenSaverFilter::ScreenSaverFilter(AppConfiguration *appConfig, QObject *parent) : QObject(parent)
{
    m_timeoutTimer.setInterval(appConfig->getInt("DISPLAY_TIMEOUT_SEC", 600) * 1000);
    m_timeoutTimer.setSingleShot(true);
    connect(&m_timeoutTimer, &QTimer::timeout, this, &ScreenSaverFilter::onTimeout);
    m_timeoutTimer.start();

    m_shutdownTimer.setInterval(appConfig->getInt("SHUTDOWN_TIMEOUT_SEC", 600) * 1000);
    m_shutdownTimer.setSingleShot(true);
    connect(&m_shutdownTimer, &QTimer::timeout, this, &ScreenSaverFilter::shutdownSystem);
}

bool ScreenSaverFilter::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QMouseEvent::MouseButtonPress || event->type()==QTouchEvent::TouchBegin) {
        qDebug() << "Input event";

        if (m_isScreensaverMode) enableScreensaver(false);

        m_timeoutTimer.start();
        m_shutdownTimer.stop();
        return false;
    } else {
        // standard event processing
        return QObject::eventFilter(obj, event);
    }
}

void ScreenSaverFilter::enableScreensaver(bool enable) {
    qDebug() << Q_FUNC_INFO;

#ifdef WIN32
    qDebug() << "Screen " << enable;
#else
    QString cmd = "vcgencmd display_power ";
    cmd.append((enable ? "0" : "1"));
    QProcess::startDetached(cmd);
#endif

    m_isScreensaverMode = enable;
}

void ScreenSaverFilter::shutdownSystem() {
#ifdef WIN32
    qDebug() << "Shutdown";
#else
    QProcess::startDetached("shutdown");
#endif
}

void ScreenSaverFilter::onTimeout() {
    qDebug() << "Screensaver active";
    enableScreensaver(true);
    m_shutdownTimer.start();
}
