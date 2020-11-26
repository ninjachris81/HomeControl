#include "ButtonController.h"
#include <LogHelper.h>
#include "Pins.h"
#include "TaskIDs.h"
#include "DisplayController.h"

ButtonController::ButtonController() : AbstractIntervalTask(BUTTON_INTERVAL_MS) {
  
}

ButtonController::~ButtonController() {
}

void ButtonController::init() {
  pinMode(PIN_BUTTON, INPUT);
}

void ButtonController::update() {
  bool val = digitalRead(PIN_BUTTON)==HIGH;

  if (lastState!=val) {
    if (lastState) {
      if (handler!=NULL) {
        handler->onPressed();
      }
    }
  }
  
  lastState = val;
}

void ButtonController::setHandler(TouchHandler *handler) {
  this->handler = handler;
}
