#include <QCoreApplication>
#include <QDebug>

void runTest(QString bitString) {
    qDebug() << bitString;

    float t = 0.0f;
    float h = 0.0f;

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
    // Calculate humidity and temp for DHT22 sensor.
    h = (data[0] * 256 + data[1]) / 10.0f;
    t = ((data[2] & 0x7F) * 256 + data[3]) / 10.0f;
    if (data[2] & 0x80) {
        t *= -1.0f;
    }

    qDebug() << t << h;
    qDebug() << (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF));
}

int main(int argc, char *argv[])
{
    //QString bitString = "1111110100101100111111110111100010100011";
    QString bitString = "0000000111111001000000000110000101011110";

    Q_ASSERT(bitString.length()==8*5);

    runTest(bitString);

    bitString = bitString.replace("1", "X");
    bitString = bitString.replace("0", "1");
    bitString = bitString.replace("X", "0");

    runTest(bitString);
}
