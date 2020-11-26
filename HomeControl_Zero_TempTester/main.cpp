#include <QCoreApplication>
#include <QDebug>
#include <QDateTime>
#include <QThread>

#include <wiringPi.h>

// Define errors and return values.
#define DHT_ERROR_TIMEOUT -1
#define DHT_ERROR_CHECKSUM -2
#define DHT_ERROR_ARGUMENT -3
#define DHT_ERROR_GPIO -4
#define DHT_SUCCESS 0


// This is the only processor specific magic value, the maximum amount of time to
// spin in a loop before bailing out and considering the read a timeout.  This should
// be a high value, but if you're running on a much faster platform than a Raspberry
// Pi or Beaglebone Black then it might need to be increased.
#define DHT_MAXCOUNT 32000

// Number of bit pulses to expect from the DHT.  Note that this is 41 because
// the first pulse is a constant 50 microsecond pulse, with 40 pulses to represent
// the data afterwards.
#define DHT_WAVES_MAX 88
#define DHT_WAVES_MIN 80
#define DHT_PULSES_EXPECTED 40

enum DHT_SENSOR {
    DHT11 = 11,
    DHT22 = 22,
    AM2302 = 22
};

void printWaves(uint32_t *values, int count) {
    QString tmp = "";
    for (int o=0;o<count;o++) {
        tmp.append(QString::number(o) + "=" + QString::number(values[o]) + " ");
    }
    qDebug() << tmp;
}

QString createBitString(uint32_t *values, int count) {
    QString tmp = "";

    for (int i = 0; i < (count/2); ++i) {
      uint32_t lowCycles = values[2 * i];
      uint32_t highCycles = values[2 * i + 1];

      if (highCycles > lowCycles) {
          tmp.append("1");
      } else {
          tmp.append("0");
      }
    }

    return tmp;
}

QString invertBits(QString bitString) {
    bitString = bitString.replace("1", "X");
    bitString = bitString.replace("0", "1");
    bitString = bitString.replace("X", "0");

    return bitString;
}

bool tryParseData(DHT_SENSOR sensor, QString bitString, float *humidity, float *temperature) {
    qDebug() << bitString;

    uint8_t data[5] = {0};

    // Inspect pulses and determine which ones are 0 (high state cycle count < low
    // state cycle count), or 1 (high state cycle count > low state cycle count).
    for (int i = 0; i < bitString.length(); ++i) {
      data[i / 8] <<= 1;
      // Now compare the low and high cycle times to see if the bit is a 0 or 1.
      if (bitString.at(i)=="1") {
        // High cycles are greater than 50us low cycle count, must be a 1.
        data[i / 8] |= 1;
      }
      // Else high cycles are less than (or equal to, a weird case) the 50us low
      // cycle count so this must be a zero.  Nothing needs to be changed in the
      // stored data.
    }

    // Useful debug info:
    printf("Data: 0x%x 0x%x 0x%x 0x%x 0x%x\n", data[0], data[1], data[2], data[3], data[4]);

    // Verify checksum of received data.
    if (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) {

        if (sensor == DHT11) {
            // Get humidity and temp for DHT11 sensor.
            *humidity = (float)data[0];
            *temperature = (float)data[2];
        }
        else if (sensor == DHT22) {
            // Calculate humidity and temp for DHT22 sensor.
            *humidity = (data[0] * 256 + data[1]) / 10.0f;
            *temperature = ((data[2] & 0x7F) * 256 + data[3]) / 10.0f;
            if (data[2] & 0x80) {
                *temperature *= -1.0f;
            }
        }

        qDebug() << humidity << temperature;

        return true;
    } else {
        return false;
    }
}

int readDHT(DHT_SENSOR sensor, int pin, float *humidity, float *temperature) {
    // Validate humidity and temperature arguments and set them to zero.
    if (humidity == nullptr || temperature == nullptr) {
        return DHT_ERROR_ARGUMENT;
    }
    *temperature = 0.0f;
    *humidity = 0.0f;

    qDebug() << Q_FUNC_INFO << QDateTime::currentMSecsSinceEpoch();

    // Store the count that each DHT bit pulse is low and high.
    // Make sure array is initialized to start at zero.
    uint32_t cycles[DHT_WAVES_MAX] = {0};

    QThread::currentThread()->setPriority(QThread::Priority::TimeCriticalPriority);

    // Set pin to output.
    pinMode(pin, OUTPUT);

    digitalWrite(pin, HIGH);
    QThread::currentThread()->msleep(1);

    // The next calls are timing critical and care should be taken
    // to ensure no unnecssary work is done below.
    digitalWrite(pin, LOW);
    QThread::currentThread()->msleep(20);

    // Set pin at input.
    pinMode(pin, INPUT);
    // Need a very short delay before reading pins or else value is sometimes still low.
    //QThread::currentThread()->usleep(2);

    /*
    qint64 timoutLoops = 0;
    while (digitalRead(pin)!=LOW) {
        if (timoutLoops > DHT_MAXCOUNT) {
            qDebug() << "DHT22 didn't pull LOW";
            return DHT_ERROR_TIMEOUT;
        }
        timoutLoops++;
    }

    timoutLoops = 0;
    while (digitalRead(pin)!=HIGH) {
        if (timoutLoops > DHT_MAXCOUNT) {
            qDebug() << "DHT22 didn't pull HIGH";
            return DHT_ERROR_TIMEOUT;
        }
        timoutLoops++;
    }
    */


    // Record pulse widths for the expected result bits.
    int wavesRead = 0;
    for (int i=0; i < DHT_WAVES_MAX; i+=2) {
        // Count how long pin is low and store in pulseCounts[i]
        while (digitalRead(pin)!=LOW) {
            if (cycles[i]>DHT_MAXCOUNT) {
                qDebug() << "Timeout while reading LOW" << i << cycles[i];

                printWaves(cycles, i);

                if (i<DHT_WAVES_MIN) {
                    return DHT_ERROR_TIMEOUT;
                } else {
                    i = DHT_WAVES_MAX+2;
                    break;
                }
            }
            cycles[i]++;
        }
        wavesRead++;

        while (digitalRead(pin)!=HIGH) {
            if (cycles[i+1]>DHT_MAXCOUNT) {
                qDebug() << "Timeout while reading HIGH" << i+1 << cycles[i+1];

                printWaves(cycles, i+1);

                if (i<DHT_WAVES_MIN) {
                    return DHT_ERROR_TIMEOUT;
                } else {
                    i = DHT_WAVES_MAX+2;
                    break;
                }
            }
            cycles[i+1]++;
        }
        wavesRead++;
    }

    // time critical over
    QThread::currentThread()->setPriority(QThread::Priority::NormalPriority);

    qDebug() << "Waves" << wavesRead;
    if ((wavesRead % 2)>0) {
        wavesRead--;
    }
    qDebug() << "Waves" << wavesRead;

    QString bitString = createBitString(cycles, wavesRead);

    qDebug() << bitString;

    if (bitString.length()==43) {
        bitString.chop(1);
    }

    bitString = bitString.mid(bitString.length() - 40);

    if (tryParseData(sensor, bitString, humidity, temperature)) {
        return DHT_SUCCESS;
    } else {
        bitString = invertBits(bitString);
        if (tryParseData(sensor, bitString, humidity, temperature)) {
            return DHT_SUCCESS;
        }
    }

    return DHT_ERROR_CHECKSUM;
}

#define DHT_TEMP_GPIO 27        // pin 13

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    wiringPiSetupGpio();

    float t = 0.0f;
    float h = 0.0f;

    while(true) {
        int res = readDHT(AM2302, DHT_TEMP_GPIO, &h, &t);

        if (res==DHT_SUCCESS) {
            qDebug() << "Temp:" << t;
            qDebug() << "Hum:" << h;
        } else {
            qWarning() << "Invalid reading from DHT" << res;
        }

        QThread::currentThread()->msleep(5000);
    }

    return a.exec();
}
