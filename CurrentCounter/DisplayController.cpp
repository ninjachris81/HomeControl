#include "DisplayController.h"
#include <LogHelper.h>
#include "ButtonController.h"

SSD1306Wire display(0x3c, 5, 4);
OLEDDisplayUi ui ( &display );

bool mqttConnected = false;
String ip = "";
double currentCurrent = 0.0;
double currentCurrentSmooth = 0.0;
uint16_t sentCount = 0;
double currentPower = 0.0;
uint16_t totalPulseCount = 0;
uint16_t adcRate = 0;

void drawFrame1(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  // Demonstrates the 3 included default sizes. The fonts come from SSD1306Fonts.h file
  // Besides the default fonts there will be a program to convert TrueType fonts into this format
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_10);

  if (mqttConnected) {
    display->drawString(0 + x, 10 + y, "Overall Power");

    display->drawString(0 + x, 20 + y, ip);
    display->setTextAlignment(TEXT_ALIGN_RIGHT);
    display->drawString(128 + x, 10 + y, String(sentCount));
    
    display->setTextAlignment(TEXT_ALIGN_CENTER);
    display->setFont(ArialMT_Plain_16);
    display->drawString(64 + x, 36 + y, String(currentPower) + " W");

    display->setTextAlignment(TEXT_ALIGN_LEFT);
    display->setFont(ArialMT_Plain_10);
    display->drawString(0 + x, 54 + y, String(totalPulseCount));
    
    display->setTextAlignment(TEXT_ALIGN_RIGHT);
    display->setFont(ArialMT_Plain_10);
    display->drawString(128 + x, 54 + y, String(adcRate));
  } else {
    display->drawString(0 + x, 10 + y, "Connecting");
  }
}

void drawFrame2(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  // Demonstrates the 3 included default sizes. The fonts come from SSD1306Fonts.h file
  // Besides the default fonts there will be a program to convert TrueType fonts into this format
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_10);

  if (mqttConnected) {
    display->drawString(0 + x, 10 + y, "Basement Power");

    display->drawString(0 + x, 20 + y, ip);
    display->setTextAlignment(TEXT_ALIGN_RIGHT);
    display->drawString(128 + x, 10 + y, String(sentCount));
    
    display->setTextAlignment(TEXT_ALIGN_CENTER);
    display->setFont(ArialMT_Plain_16);
    display->drawString(64 + x, 36 + y, String(currentCurrent * 230.0) + " W");
    
    display->setFont(ArialMT_Plain_10);
    display->drawString(64 + x, 54 + y, String(currentCurrentSmooth * 230.0) + " W");

  } else {
    display->drawString(0 + x, 10 + y, "Connecting");
  }
}

FrameCallback frames[] = { drawFrame1, drawFrame2 };
int frameCount = 2;

DisplayController::DisplayController() : AbstractIntervalTask(DISPLAY_UPDATE_INTERVAL) {
  
}

DisplayController::~DisplayController() {
}

void DisplayController::init() {
  taskManager->getTask<ButtonController*>(BUTTON_CONTROLLER)->setHandler(this);
  
  ui.setTargetFPS(1);
  ui.disableAutoTransition();
  ui.disableAllIndicators();
  ui.setTimePerTransition(0);

  ui.setFrames(frames, frameCount);

  ui.init();

  display.flipScreenVertically();
}

void DisplayController::update() {
  ui.update();

  if (millis() - lastClick > DISPLAY_TIMEOUT_MS) {
    displayIsOn = false;
    display.displayOff();
  }
}

void DisplayController::setMqttConnected(bool newValue, String newIP) {
  mqttConnected = newValue;
  ip = newIP;
}

void DisplayController::setCurrent(double newCurrent, double newCurrentSmooth) {
  currentCurrent = newCurrent;
  currentCurrentSmooth = newCurrentSmooth;
}

void DisplayController::setPower(double newPower, uint16_t newPulseCount, uint16_t newAdcRate) {
  currentPower = newPower;
  totalPulseCount = newPulseCount;
  adcRate = newAdcRate;
}

void DisplayController::increaseSentCount() {
  sentCount++;
}

void DisplayController::onPressed() {
  lastClick = millis();
  if (!displayIsOn) {
    display.displayOn();
    displayIsOn = true;
  } else {
    ui.nextFrame();
  }
}
