#include "utils/rpianalogreader.h"
#include "utils/wiringpimanager.h"
#include <stdio.h>

#ifdef USE_WIRING_PI
    #include <wiringPi.h>
    #include <ads1115.h>
#endif

bool RPIAnalogReader::isInitialized = false;

RPIAnalogReader::RPIAnalogReader(QObject *parent) : QObject(parent)
{
    WiringPiManager::initWiringPi();

    if (!isInitialized) {
#ifdef USE_WIRING_PI
    ads1115Setup(AD_BASE,0x48);
#endif
        isInitialized = true;
    }

}

void RPIAnalogReader::configure(int channel, int gain) {
    m_gains[channel] = gain;
}

int RPIAnalogReader::readValue(int channel) {
#ifdef USE_WIRING_PI
    digitalWrite(AD_BASE, m_gains[channel]);
    return analogRead(AD_BASE+channel);
#else
    Q_UNUSED(channel)
    return 0;
#endif
}
