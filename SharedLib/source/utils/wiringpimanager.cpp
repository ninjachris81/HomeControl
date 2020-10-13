#include "include/utils/wiringpimanager.h"
#ifdef USE_WIRING_PI
    #include <wiringPi.h>
#endif

bool WiringPiManager::isInitialized = false;

WiringPiManager::WiringPiManager(QObject *parent) : QObject(parent)
{

}

void WiringPiManager::initWiringPi() {
    if (isInitialized) return;
#ifdef USE_WIRING_PI
    wiringPiSetup();
#endif
    isInitialized = true;
}
