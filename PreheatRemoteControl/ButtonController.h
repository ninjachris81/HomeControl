#ifndef BUTTONCONTROLLER_H
#define BUTTONCONTROLLER_H

#include <AbstractIntervalTask.h>
#include <Property.h>

#define BUTTON_CHECK_INTERVAL 200

#define BUTTON_LONG_PRESS 5000

class ButtonController : public AbstractIntervalTask, public Property<bool>::ValueChangeListener  {
public:
  ButtonController();

  void init();

  void update();

  void onPropertyValueChange(uint8_t id, bool newValue, bool oldValue);
private:
  uint64_t pressStarted = 0;

  Property<bool> buttonPressed;

};

#endif  // BUTTONCONTROLLER_H
