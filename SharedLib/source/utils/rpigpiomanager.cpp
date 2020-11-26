#include "include/utils/rpigpiomanager.h"
#include "include/utils/wiringpimanager.h"
#ifdef USE_WIRING_PI
    #include <wiringPi.h>
#endif

Q_LOGGING_CATEGORY(LG_RPI_GPIO_MANAGER, "RpiGpiManager");

RPIGpioManager::RPIGpioManager(QObject *parent) : QObject(parent)
{
    WiringPiManager::initWiringPi();
}

void RPIGpioManager::configureAsInput(int channel) {
#ifdef USE_WIRING_PI
    qCDebug(LG_RPI_GPIO_MANAGER) << "Configure" << channel << "as input";
    pinMode(channel, INPUT);
#else
    qCDebug(LG_RPI_GPIO_MANAGER) << "Ignoring configure as input" << channel;
    Q_UNUSED(channel)
#endif
}
void RPIGpioManager::configureAsOutput(int channel) {
#ifdef USE_WIRING_PI
    qCDebug(LG_RPI_GPIO_MANAGER) << "Configure" << channel << "as output";
    pinMode(channel, OUTPUT);
#else
    qCDebug(LG_RPI_GPIO_MANAGER) << "Ignoring configure as output" << channel;
    Q_UNUSED(channel)
#endif
}

bool RPIGpioManager::read(int channel) {
#ifdef USE_WIRING_PI
    //qCDebug(LG_RPI_GPIO_MANAGER) << "Read" << channel;
    return digitalRead(channel);
#else
    qCDebug(LG_RPI_GPIO_MANAGER) << "Ignoring read" << channel;
    Q_UNUSED(channel)
    return false;
#endif
}

void RPIGpioManager::write(int channel, bool value) {
#ifdef USE_WIRING_PI
    //qCDebug(LG_RPI_GPIO_MANAGER) << "Write" << channel;
    digitalWrite(channel, value);
#else
    qCDebug(LG_RPI_GPIO_MANAGER) << "Ignoring write" << channel;
    Q_UNUSED(channel)
    Q_UNUSED(value)
#endif
}
