#ifndef DHT_UTILS_H
#define DHT_UTILS_H

#include <QObject>

#include "rpigpiomanager.h"

// Define errors and return values.
#define DHT_ERROR_TIMEOUT -1
#define DHT_ERROR_CHECKSUM -2
#define DHT_ERROR_ARGUMENT -3
#define DHT_ERROR_GPIO -4
#define DHT_SUCCESS 0

class DhtUtils {
public:
    enum DHT_SENSOR {
       DHT11 = 11,
       DHT22 = 22,
       AM2302 = 22
    };

    int readDHT(DHT_SENSOR sensor, int pin, float* humidity, float* temperature);

private:
    RPIGpioManager gpioManager;

};
#endif
