#include "ButtonController.h"

#include <LogHelper.h>

#include "Pins.h"
#include "TaskIDs.h"
#include "LogicController.h"

ButtonController::ButtonController() : AbstractIntervalTask(BUTTON_CHECK_INTERVAL) {
}

void ButtonController::init() {
  pinMode(PIN_BUTTON, INPUT);

  buttonPressed.init(0, false);
  buttonPressed.registerValueChangeListener(this);
}

void ButtonController::update() {
  buttonPressed.setValue(analogRead(PIN_BUTTON)>900);

  if (pressStarted>0 && buttonPressed.getValue() && (millis() - pressStarted > BUTTON_LONG_PRESS)) {
    taskManager->getTask<LogicController*>(LOGIC_CONTROLLER)->stopHeating();
    pressStarted = 0;
  }
}

void ButtonController::onPropertyValueChange(uint8_t id, bool newValue, bool oldValue) {
  LOG_PRINT(F("Button "));
  LOG_PRINTLN(newValue);
  
  if (newValue) {
    pressStarted = millis();
  } else {
    if (pressStarted > 0 && (millis() - pressStarted < BUTTON_LONG_PRESS)) {
      taskManager->getTask<LogicController*>(LOGIC_CONTROLLER)->startHeating();
    } else {
      // ignore long press, already handled
    }
    pressStarted = 0;
  }
}
