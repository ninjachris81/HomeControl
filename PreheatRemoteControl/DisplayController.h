#ifndef DISPLAYCONTROLLER_H
#define DISPLAYCONTROLLER_H

#include <AbstractTriggerTask.h>
#include <LiquidCrystal_I2C.h>
#include <Property.h>

#define CUSTOM_CHAR_INDEX_DEGREE 0
#define CUSTOM_CHAR_INDEX_CONNECTED 1
#define CUSTOM_CHAR_INDEX_DISCONNECTED 2

class DisplayController : public AbstractTriggerTask, public Property<uint8_t>::ValueChangeListener  {
public:
  DisplayController();

  enum DISPLAY_PROPERTY {
    PROP_TEMP_WATER,
    PROP_TEMP_HC,
    PROP_TEMP_TANK,
    PROP_PUMP_WATER,
    PROP_PUMP_HC,
    PROP_TS,

    // extra
    PROP_IS_CONNECTED,

    PROP_COUNT
  };

  void init();

  void update();

  void setValue(DISPLAY_PROPERTY prop, uint8_t value);

  void printActiveState(bool enabled);

  void printStartingState();

  void setActive(bool active);
  
  void onPropertyValueChange(uint8_t id, uint8_t newValue, uint8_t oldValue);

private:
  LiquidCrystal_I2C *lcd;

  Property<uint8_t> props[PROP_COUNT];

  uint8_t degree[8] = {  0xe,0x11,0x11,0xe,0x0,0x0,0x0};
  uint8_t isConnected[8] = {  0x0,0x4,0xe,0x1f,0xe,0x4,0x0};
  uint8_t isDisconnected[8] = {  0x4,0xe,0x1f,0x0,0x1f,0xe,0x4};

  void printNumber2(uint8_t number);
  void printNumber4(uint16_t number, bool printZero);

};

#endif    //DISPLAYCONTROLLER_H
