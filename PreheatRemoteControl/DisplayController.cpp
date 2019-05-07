#include "DisplayController.h"

#include "TaskIDs.h"
#include "LogicController.h"
#include "MqttController.h"

DisplayController::DisplayController() : AbstractTriggerTask() {
}

void DisplayController::init() {

  for (uint8_t i=0;i<PROP_COUNT;i++) {
    props[i].init(i, 0);
    props[i].registerValueChangeListener(this);
  }
  
  lcd = new LiquidCrystal_I2C(0x27,16,2);

  lcd->init();

  lcd->clear();
  lcd->createChar(CUSTOM_CHAR_INDEX_DEGREE, degree);
  lcd->createChar(CUSTOM_CHAR_INDEX_CONNECTED, isConnected);
  lcd->createChar(CUSTOM_CHAR_INDEX_DISCONNECTED, isDisconnected);

  setActive(true);
  triggerUpdate();
}

void DisplayController::update() {
  switch(taskManager->getTask<LogicController*>(LOGIC_CONTROLLER)->getState()) {
    case LogicController::STATE_STARTING:
      printStartingState();
      break;
    case LogicController::STATE_RUNNING:
      printActiveState(true);
      break;
    case LogicController::STATE_STOPPED:
      printActiveState(false);
      break;
  }
}

void DisplayController::setValue(DISPLAY_PROPERTY prop, uint8_t value) {
  props[prop].setValue(value);
}

void DisplayController::onPropertyValueChange(uint8_t id, uint8_t newValue, uint8_t oldValue) {
  triggerUpdate();
}

void DisplayController::printStartingState() {
  lcd->clear();
  lcd->setCursor(0,0);
  lcd->print(F("TANK  WT  WASSER"));
 
  lcd->setCursor(0,1);
  if (taskManager->getTask<MqttController*>(MQTT_CONTROLLER)->isConnected()) {
    lcd->print(F("   VERBINDE..."));
  } else {
    lcd->print(F("CLIENT VERBUNDEN"));
  }
}

void DisplayController::printActiveState(bool enabled) {
  lcd->clear();
  int tempWater = props[PROP_TEMP_WATER].getValue();
  int tempHC = props[PROP_TEMP_HC].getValue();
  int tempTank = props[PROP_TEMP_TANK].getValue();
  bool pumpWater = props[PROP_PUMP_WATER].getValue()==1;
  bool pumpHC = props[PROP_PUMP_HC].getValue()==1;
  bool isConnected = props[PROP_IS_CONNECTED].getValue()==1;
  
  // update display
  if (tempWater==-1 && tempHC==-1) {
    /*
    lcd->setCursor(8,0);
    lcd->print("      ");
    lcd->setCursor(8,1);
    lcd->print("      ");
    */
  } else {
    lcd->setCursor(0,0);

    if (isConnected) {
      lcd->write(CUSTOM_CHAR_INDEX_CONNECTED);
    } else {
      lcd->write(CUSTOM_CHAR_INDEX_DISCONNECTED);
    }
    
    lcd->print(" ");
    printNumber2(tempTank);
    lcd->write(CUSTOM_CHAR_INDEX_DEGREE);
  
    lcd->print("  ");
    printNumber2(tempHC);
    lcd->write(CUSTOM_CHAR_INDEX_DEGREE);

    lcd->print("  ");
    printNumber2(tempWater);
    lcd->write(CUSTOM_CHAR_INDEX_DEGREE);

    lcd->setCursor(0,1);
    if (enabled) {
      lcd->print("HEIZE");
    } else {
      lcd->print("     ");
    }
    
    lcd->setCursor(7,1);
    lcd->print(pumpHC ? F("AN ") : F("AUS"));
    lcd->print("  ");
    lcd->print(pumpWater ? F("AN ") : F("AUS"));
  }
}

void DisplayController::printNumber2(uint8_t number) {
  if (number<10) lcd->print(" ");
  lcd->print(number);
}

void DisplayController::printNumber4(uint16_t number, bool printZero) {
  if (number<1000) lcd->print(" ");
  if (number<100) lcd->print(" ");
  if (number<10) lcd->print(" ");
  if (number==0 && !printZero) {
    lcd->print(" ");
  } else {
    lcd->print(number);
  }
}

void DisplayController::setActive(bool active) {
  if (active) {
    lcd->backlight();
  } else {
    lcd->noBacklight();
  }
}
