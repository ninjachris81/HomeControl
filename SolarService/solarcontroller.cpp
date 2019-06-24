#include "solarcontroller.h"
#include <QDebug>
#ifdef __linux__
    #include <wiringPi.h>
#endif

SolarController::SolarController(QObject *parent) : QObject(parent)
{
    mMaintenanceTimer.setInterval(1000);
    connect(&mMaintenanceTimer, &QTimer::timeout, this, &SolarController::onMaintenance);
}

bool SolarController::init() {
    qDebug() << Q_FUNC_INFO;

#ifdef __linux__
    if (wiringPiSetup() == -1) return false;

    pinMode(PIN_MOTOR_RELAY, OUTPUT);
#endif

    initStarted = QDateTime::currentDateTime();

    mMaintenanceTimer.start();

    return true;
}

void SolarController::switchRelay(MotorDirection direction) {
    qDebug() << Q_FUNC_INFO << direction;

#ifdef __linux__
    digitalWrite(PIN_MOTOR_RELAY, true);
#endif
}

void SolarController::onMaintenance() {
    qDebug() << initStarted.secsTo(QDateTime::currentDateTime());

    if (initStarted.secsTo(QDateTime::currentDateTime()) < 120) {
        // init phase
        qDebug() << "Init";
        switchRelay(DIRECTION_DOWN);
        m_currentPosition = POSITION_INITIAL;
    } else {
        int h = QDateTime::currentDateTime().time().hour();
        if (h>
    }
}
