#ifndef SCREENSAVERFILTER_H
#define SCREENSAVERFILTER_H

#include <QObject>
#include <QTimer>
#include "utils/appconfiguration.h"

class ScreenSaverFilter : public QObject
{
    Q_OBJECT
public:
    explicit ScreenSaverFilter(AppConfiguration *appConfig, QObject *parent = nullptr);

protected:
    bool m_isScreensaverMode = false;
    QTimer m_timeoutTimer;
    QTimer m_shutdownTimer;

    bool eventFilter(QObject *obj, QEvent *event) override;

    void enableScreensaver(bool enable);

protected slots:
    void onTimeout();

    void shutdownSystem();

signals:

public slots:
};

#endif // SCREENSAVERFILTER_H
