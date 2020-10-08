#include "DisplayController.h"
#include <LogHelper.h>

SSD1306Wire display(0x3c, 5, 4);
OLEDDisplayUi ui ( &display );

bool mqttConnected = false;
String ip = "";
double currentCurrent = 0.0;
double currentCurrentSmooth = 0.0;
uint16_t sentCount = 0;

void drawFrame1(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  // Demonstrates the 3 included default sizes. The fonts come from SSD1306Fonts.h file
  // Besides the default fonts there will be a program to convert TrueType fonts into this format
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_10);

  if (mqttConnected) {
    display->drawString(0 + x, 10 + y, "Connected");

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

FrameCallback frames[] = { drawFrame1 };
int frameCount = 1;

DisplayController::DisplayController() : AbstractTask() {
  
}

DisplayController::~DisplayController() {
}

void DisplayController::init() {
  ui.setTargetFPS(2);
  ui.disableAutoTransition();
  ui.disableAllIndicators();

  ui.setFrames(frames, frameCount);

  ui.init();

  display.flipScreenVertically();
}

void DisplayController::update() {
  ui.update();
}

void DisplayController::setMqttConnected(bool newValue, String newIP) {
  mqttConnected = newValue;
  ip = newIP;
}

void DisplayController::setCurrent(double newCurrent, double newCurrentSmooth) {
  currentCurrent = newCurrent;
  currentCurrentSmooth = newCurrentSmooth;
}

void DisplayController::increaseSentCount() {
  sentCount++;
}
