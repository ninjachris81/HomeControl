#ifndef BUTTON_CONTROLLER_H
#define BUTTON_CONTROLLER_H

#include <AbstractIntervalTask.h>

#define BUTTON_INTERVAL_MS 100

class ButtonController : public AbstractIntervalTask {
public:
  ButtonController();
  ~ButtonController();

  class TouchHandler {
    public:
      virtual void onPressed() = 0;
  };


  void init();

  void update();

  void setHandler(TouchHandler *handler);
  
private:
  bool lastState = false;

  TouchHandler* handler = NULL;
};


#endif
