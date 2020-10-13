#include "include/utils/rpigpiomanager.h"
#include "include/utils/wiringpimanager.h"
#ifdef USE_WIRING_PI
    #include <wiringPi.h>
#endif


RPIGpioManager::RPIGpioManager(QObject *parent) : QObject(parent)
{
    WiringPiManager::initWiringPi();
}

void RPIGpioManager::configureAsInput(int channel) {
#ifdef USE_WIRING_PI
    pinMode(channel, INPUT);
#else
    Q_UNUSED(channel)
#endif
}
void RPIGpioManager::configureAsOutput(int channel) {
#ifdef USE_WIRING_PI
    pinMode(channel, OUTPUT);
#else
    Q_UNUSED(channel)
#endif
}

bool RPIGpioManager::read(int channel) {
#ifdef USE_WIRING_PI
    return digitalRead(channel);
#else
    Q_UNUSED(channel)
    return false;
#endif
}

void RPIGpioManager::write(int channel, bool value) {
#ifdef USE_WIRING_PI
    digitalWrite(channel, value);
#else
    Q_UNUSED(channel)
    Q_UNUSED(value)
#endif
}
